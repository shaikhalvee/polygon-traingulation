#include<bits/stdc++.h>
#define MAX 10000000
#define pi (2*acos(0.0))
using namespace std;

struct point
{
    int x, y;
    char type;
    int id;
    point()
    {

    }
    point(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
};

struct edge
{
    double x1,x2,y1,y2;
    int id;

    edge(double x1,double x2,double y1,double y2)
    {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }
    edge(point p1, point p2, int i)
    {
        x1 = p1.x;
        y1 = p1.y;
        x2 = p2.x;
        y2 = p2.y;
        id = i;
    }
};

struct diagonal
{
    point p1, p2;

    diagonal(point a, point b)
    {
        p1 = a;
        p2 = b;
    }
};

typedef vector<point> vp;
typedef vector<diagonal> vd;
typedef vector<edge> ve;
vp points;
vp helper;
ve edgeList;
vd diagonal_list;
int y_sweepLine;
set<edge> edgeSet;
int size;

bool operator < (const edge& left, const edge& right);
void print(vp P);
int crossProduct(point, point, point);
void setTypes(vp &P);
vd makeMonotone();
void handleStartVertex(point Vi);
void handleEndVertex(point Vi);
void handleSplitVertex(point Vi);
void handleMergeVertex(point Vi);
void handleRegVertex(point Vi);

int main()
{
    FILE *fp = freopen("input.txt","r",stdin);
    point input;
    vp sorted_points;
    cin >> size;
    for(int i = 0; i<size; i++){
        cin >> input.x >> input.y;
        input.id = i;
        points.push_back(input);
    }
    // points type set
    setTypes(points);
    helper.assign(size, point());
    for(int i=0;i<points.size()-1;++i){
        edge temp(points[i], points[i+1], i);
        edgeList.push_back(temp);
    }
    edge temp(points[points.size()-1], points[0], points.size());
    edgeList.push_back(temp);
    sorted_points = points;
    // sort by y co cordinate
    sort(sorted_points.begin(), sorted_points.end(), [](const point& upPoint, const point& downPoint)
    {
        return upPoint.y == downPoint.y ? upPoint.x < downPoint.x : upPoint.y > downPoint.y ;
    });
    //print(points);
    return 0;
}

bool operator < (const edge& left, const edge& right)
{
    double left_x, right_x, leftM, rightM;
    leftM = (left.x1 - left.x2) / (left.y1 - left.y2);
    rightM = (right.x1 - right.x2) / (right.y1 - right.y2);
    left_x = leftM * (y_sweepLine - left.y1);
    right_x = rightM * (y_sweepLine - right.y1);
    return left_x < right_x;
}

int nextVtx(int id)
{
    return (id + 1) % size;
}

int prevVtx(int id)
{
    return (id - 1 + size) % size;
}

void print(vp P)
{
    cout<<endl;
    for(int i = 0; i<P.size(); i++){
        cout << P[i].x << " " << P[i].y << endl;
        cout << "type : " << P[i].type << endl;
    }
}

// for points a,b,c return (ac x ab)
int crossProduct(point a, point b, point c)
{
    int ac_x = c.x-a.x;
    int ac_y = c.y-a.y;
    int ab_x = b.x-a.x;
    int ab_y = b.y-a.y;
    return ac_x*ab_y - ac_y*ab_x;
}

void setTypes(vp &P)
{
    point now,next,prev;
    for(int i=0; i<P.size(); ++i){
        now = P[i];
        next = P[i+1];
        prev =  P[ ( i-1 + P.size() ) % P.size() ];
        if(now.y > next.y && now.y > prev.y){       // start or split
            if( crossProduct(now, prev, next)>0 ){  // start
                P[i].type = 's';
            }
            else{                                   // split
                P[i].type = 'p';
            }
        }
        else if(now.y < next.y && now.y < prev.y){  // end or merge
            if(crossProduct(now, prev, next)>0){    // end
                P[i].type = 'e';
            }
            else{
                P[i].type = 'm';                    // merge
            }
        }
        else{
            P[i].type = 'r';                        // regular
        }
    }
}

vd makeMonotone()
{
    for(int i=0;i<points.size();i++){
        switch(points[i].type)
        {
        case 's':
            handleStartVertex(points[i]);
            break;
        case 'e':
            handleEndVertex(points[i]);
            break;
        case 'p':
            handleSplitVertex(points[i]);
            break;
        case 'm':
            handleMergeVertex(points[i]);
            break;
        case 'r':
            handleRegVertex(points[i]);
            break;
        }
    }
}

void handleStartVertex(point Vi)
{
    edgeSet.insert(edgeList[Vi.id]);    // Vi - insert - Ei
    helper[edgeList[Vi.id].id] = Vi;    // helper(ei) = vi
}

void handleEndVertex(point Vi)
{
    int prevVtxID = prevVtx(Vi.id) ;
    int curEdgeId = edgeList[prevVtxID].id;
    if(helper[curEdgeId].type == 'm'){
        diagonal now(Vi, helper[curEdgeId]);
        diagonal_list.push_back(now);
    }
    edge temp = edgeList[curEdgeId];
    set<edge>::iterator removePtr = edgeSet.find(temp);
    edgeSet.erase(removePtr);
}

void handleSplitVertex(point Vi)
{
    edge temp(Vi, Vi, Vi.id);
    set<edge>::iterator leftEdge = edgeSet.upper_bound(temp);
    leftEdge--;
    point nextPoint = helper[leftEdge->id];
    diagonal now(Vi, nextPoint);
    diagonal_list.push_back(now);
    helper[leftEdge->id] = Vi;
    edgeSet.insert(edgeList[Vi.id]);
    helper[edgeList[Vi.id].id] = Vi;
}

void handleMergeVertex(point Vi)
{
    int prevVtxID = prevVtx(Vi.id);        // i-1 of vi
    int curEdgeId = edgeList[prevVtxID].id;   // i of ei
    if(helper[curEdgeId].type == 'm'){
        diagonal now(Vi, helper[curEdgeId]);
        diagonal_list.push_back(now);
    }
    edge temp = edgeList[curEdgeId];
    set<edge>::iterator removePtr = edgeSet.find(temp);
    edgeSet.erase(removePtr);
    edge temp(Vi, Vi, Vi.id);
    set<edge>::iterator leftEdge = edgeSet.upper_bound(temp);
    leftEdge--;
    if(helper[leftEdge->id].type == 'm'){
        diagonal now(Vi, helper[leftEdge->id]);
        diagonal_list.push_back(now);
    }
    helper[leftEdge->id] = Vi;
}

void handleRegVertex(point Vi)
{

}

/*
for(set<edge>::iterator itr = edgeList.begin(); itr != edgeList.end(); ++itr)
{
    cout << itr->x1 << " " << itr->y1 << " " << itr->x2 << " " << itr->y2 << endl;
}
*/
