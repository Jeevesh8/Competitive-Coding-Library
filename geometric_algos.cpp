#include<iostream>
#include<limits>
#include<vector>
#include<stack>
using namespace std ;

#define ll long long int 
#define p_infty(t) +numeric_limits<t>::infinity()

ll orientation(point_t o, ponit_t a, point_t b)
{
    /*
    Calculates orientation between line segments oa and ob
    */
    ll cross_prod = ((a.x-o.x)*(b.y-o.y)) - ((a.y-o.y)*(b.x-o.x)) ;
    if(cross_prod==0)   return 0 ;      //Co-Linear
    return cross_prod>0 ? 2 : 1 ;       //1--clockwise ; 2--anti-clockwise
}

bool on_segment(point_t p, point_t q, point_t r)
{
    /*
    p,q,r are co-linear points. Returns true if r lines on line segment joining p and q.
    */
    if( r.x<=max(p.x, q.x) && r.x>=min(p.x, q.x) && r.y<=max(p.y, q.y) && r.y<=min(p.y, q.y) )
        return true ;
    return false ;
}

bool intersect(point_t a, point_t b, point_t q, point_t r)
{
    /*
    Returns true if the line joining a,b and line joining c,d intersect .
    */
    ll o1 = orientation(a, b, q) ;
    ll o2 = orientation(a, b, r) ;
    ll o3 = orientation(q, r, a) ;
    ll o4 = orientation(q, r, b) ;
    
    if(o1!=o2&&o3!=o4)  return true ;                                           //Lines form an X

    if(o1==o2&&o3==o4&&o1!=0)   return false ;                                  //(a,b) lie on same side of extended qr and vice-versa

    if( (o1==0||o2==0) && (on_segment(a, b, q) || on_segment(a, b, r)) )        //at least one of (q,r) lies on ab
            return true ;
    if( (o3==0||o4==0) && (on_segment(q, r, a) || on_segmentq(q, r, b)) )       //at least one of (a,b) lies on qr
            return true ;   
    return false ;                                                              //extended ab == extended qr == l . ab, qr are distinct parts of l.
}

bool is_inside(vector<point_t>& polygon, point_t p)
{
    /*
    Returns true if point p lies inside or on the polygon . Otherwise, returns false. 
    Time Complexity = O(polygon.size())
    */
    point_t right_inf ;
    right_inf.x = p_infty(type(point_t::x)) ;
    right_inf.y = p.y ;

    size_t count = 0, n = polygon.size();
    point_t next ;
    for(size_t i=0; i<n; ++i)
    {
        next = (i+1)%n ;
        if(intersect(p, right_inf, polygon[i], polygon[next]))
        {
            count++ ;
            if(orientation(p, polygon[i], polygon[next])==0)    return true ;
        }
    }
    return count&1 ;
}

//Convex-Hull Using Graham-Scan . Time complexity = O(nlogn)
vector<point_t>& convex_hull(vector<point_t>& points)
{
    if(points.size()<=3)    return points ;

    point_t first_point = *min_element(points.begin(), points.end(), [](point_t p1, point_t p2)
                                                                        {
                                                                            if(p1.y<p2.y) return true ;
                                                                            if(p2.y<p1.y) return false ;
                                                                            if(p1.x<p2.x) return true ;
                                                                            return false ;
                                                                        }
                                                                        ) ;
    
    vector<point_t> angle_sorted_points = points ;
    sort(angle_sorted_points.begin(), angle_sorted_points.end(), [](point_t p1, point_t p2)
                                                                    {
                                                                        if((p1.y-first_point.y)*(p2.x-first_point.x)<(p2.y-first_point.y)*(p1.x-first_point.x))  return true ;
                                                                        if((p1.y-first_point.y)*(p2.x-first_point.x)>(p2.y-first_point.y)*(p1.x-first_point.x))  return false ;
                                                                        if(p1.x<p2.x)   return true ;
                                                                        return false ;
                                                                    }) ;
    point_t p1 = angle_sorted_points[0], p2 = angle_sorted_points[1] ;
    point_t p3 ; 
    vector<point_t> convex_hull ;
    convex_hull.push_back(p1) ;
    convex_hull.push_back(p2) ;
    for(size_t i=2; i<angle_sorted_points.size(); ++i)
    {
        size_t m = convex_hull.size() ;
        ll o = orientation(convex_hull[m-1], convex_hull[m-2], angle_sorted_points[i]) ;
        if(o==2)    convex_hull.pop_back() ;
        convex_hull.push_back(angle_sorted_points[i]) ;
    }
    return convex_hull ;
}