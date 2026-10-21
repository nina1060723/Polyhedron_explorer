#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
constexpr double PI = 3.14159265358979323846;
struct V { double x,y,z; V operator+(V b)const{return{x+b.x,y+b.y,z+b.z};} V operator-(V b)const{return{x-b.x,y-b.y,z-b.z};} V operator*(double s)const{return{x*s,y*s,z*s};} V operator/(double s)const{return{x/s,y/s,z/s};} };
double dot(V a,V b){return a.x*b.x+a.y*b.y+a.z*b.z;} V cross(V a,V b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};} double norm(V a){return sqrt(dot(a,a));}
struct Solid { vector<V> v; vector<vector<int>> f; };
V centroid(const Solid& s){V c{0,0,0};for(auto v:s.v)c=c+v;return c/double(s.v.size());}
void orient(Solid& s){V c=centroid(s);for(auto &f:s.f){V n=cross(s.v[f[1]]-s.v[f[0]],s.v[f[2]]-s.v[f[0]]);if(dot(n,s.v[f[0]]-c)<0)reverse(f.begin(),f.end());}}
Solid regular(int faces,double edge){
 Solid s;
 if(faces==4){s.v={{1,1,1},{1,-1,-1},{-1,1,-1},{-1,-1,1}};s.f={{0,1,2},{0,3,1},{0,2,3},{1,3,2}};}
 else if(faces==6){s.v={{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}};s.f={{0,3,2,1},{4,5,6,7},{0,1,5,4},{1,2,6,5},{2,3,7,6},{3,0,4,7}};}
 else if(faces==8){s.v={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};s.f={{0,2,4},{2,1,4},{1,3,4},{3,0,4},{2,0,5},{1,2,5},{3,1,5},{0,3,5}};}
 else if(faces==20||faces==12){double p=(1+sqrt(5.0))/2;Solid ico;ico.v={{-1,p,0},{1,p,0},{-1,-p,0},{1,-p,0},{0,-1,p},{0,1,p},{0,-1,-p},{0,1,-p},{p,0,-1},{p,0,1},{-p,0,-1},{-p,0,1}};ico.f={{0,11,5},{0,5,1},{0,1,7},{0,7,10},{0,10,11},{1,5,9},{5,11,4},{11,10,2},{10,7,6},{7,1,8},{3,9,4},{3,4,2},{3,2,6},{3,6,8},{3,8,9},{4,9,5},{2,4,11},{6,2,10},{8,6,7},{9,8,1}};orient(ico);
 if(faces==20)s=ico;
 else { // Dual of icosahedron: face centroids become dodecahedron vertices.
  for(auto f:ico.f)s.v.push_back((ico.v[f[0]]+ico.v[f[1]]+ico.v[f[2]])/3.0);
  for(int vi=0;vi<(int)ico.v.size();++vi){vector<int> adjacent;for(int fi=0;fi<(int)ico.f.size();++fi)if(find(ico.f[fi].begin(),ico.f[fi].end(),vi)!=ico.f[fi].end())adjacent.push_back(fi);
   V n=ico.v[vi]/norm(ico.v[vi]);V axis=cross(n,{0,0,1});if(norm(axis)<1e-8)axis=cross(n,{0,1,0});axis=axis/norm(axis);V other=cross(n,axis);
   sort(adjacent.begin(),adjacent.end(),[&](int a,int b){V da=s.v[a]-ico.v[vi],db=s.v[b]-ico.v[vi];return atan2(dot(da,other),dot(da,axis))<atan2(dot(db,other),dot(db,axis));});s.f.push_back(adjacent);
  }
 }
 } else throw runtime_error("Faces must be 4, 6, 8, 12, or 20.");
 orient(s);double current=norm(s.v[s.f[0][0]]-s.v[s.f[0][1]]);for(auto &v:s.v)v=v*(edge/current);return s;
}
struct Stats{int vertices,edges,faces;double area,volume;};
Stats stats(const Solid& s){set<pair<int,int>> edges;double area=0,volume=0;for(auto f:s.f){for(size_t i=0;i<f.size();++i){int a=f[i],b=f[(i+1)%f.size()];edges.insert(minmax(a,b));}for(size_t i=1;i+1<f.size();++i){V a=s.v[f[0]],b=s.v[f[i]],c=s.v[f[i+1]];area+=norm(cross(b-a,c-a))/2;volume+=dot(a,cross(b,c))/6;}}return {(int)s.v.size(),(int)edges.size(),(int)s.f.size(),area,abs(volume)};}
// Clip every convex face against one half-space, then close the new cut face.
Solid clip(const Solid& src,V normal,double offset,bool positive,vector<V>& section){Solid out;double scale=1;for(V v:src.v)scale=max(scale,norm(v));double eps=1e-9*scale;
 auto dist=[&](V v){double d=dot(normal,v)-offset;return positive?d:-d;};
 auto add=[&](V v){for(int i=0;i<(int)out.v.size();++i)if(norm(out.v[i]-v)<eps)return i;out.v.push_back(v);return (int)out.v.size()-1;};
 for(auto face:src.f){vector<int> polygon;for(size_t j=0;j<face.size();++j){V a=src.v[face[j]],b=src.v[face[(j+1)%face.size()]];double da=dist(a),db=dist(b);if(da>=-eps)polygon.push_back(add(a));if((da>eps&&db< -eps)||(da< -eps&&db>eps)){V p=a+(b-a)*(da/(da-db));polygon.push_back(add(p));bool found=false;for(V q:section)if(norm(q-p)<eps){found=true;break;}if(!found)section.push_back(p);}}
 vector<int> cleaned;for(int k:polygon)if(cleaned.empty()||cleaned.back()!=k)cleaned.push_back(k);if(cleaned.size()>1&&cleaned.front()==cleaned.back())cleaned.pop_back();if(cleaned.size()>=3)out.f.push_back(cleaned);
 }
 return out;
}
struct Cut {Solid a,b;double sectionArea;bool valid;};
Cut cut(const Solid& src,V normal,double offset){double length=norm(normal);if(length<1e-12)throw runtime_error("Plane normal cannot be zero.");normal=normal/length;offset/=length;
 double lo=1e100,hi=-1e100;for(V v:src.v){double d=dot(normal,v)-offset;lo=min(lo,d);hi=max(hi,d);}double scale=1;for(V v:src.v)scale=max(scale,norm(v));if(lo>=-1e-9*scale||hi<=1e-9*scale)return {{},{},0,false};
 vector<V> points;Solid a=clip(src,normal,offset,true,points);vector<V> discard;Solid b=clip(src,normal,offset,false,discard);
 if(points.size()<3) return {{},{},0,false};
 V center{0,0,0};for(V p:points)center=center+p;center=center/double(points.size());V u=cross(normal,{0,0,1});if(norm(u)<1e-8)u=cross(normal,{0,1,0});u=u/norm(u);V v=cross(normal,u);
 sort(points.begin(),points.end(),[&](V p,V q){V dp=p-center,dq=q-center;return atan2(dot(dp,v),dot(dp,u))<atan2(dot(dq,v),dot(dq,u));});
 double area=0;for(size_t i=0;i<points.size();++i)area+=dot(normal,cross(points[i]-center,points[(i+1)%points.size()]-center))/2;area=abs(area);
 auto close=[&](Solid& s){vector<int> f;for(V p:points){int idx=-1;for(int i=0;i<(int)s.v.size();++i)if(norm(s.v[i]-p)<1e-8*scale){idx=i;break;}if(idx<0){idx=(int)s.v.size();s.v.push_back(p);}f.push_back(idx);}s.f.push_back(f);orient(s);};
 close(a);close(b);return {a,b,area,true};
}
int main(){cout<<fixed<<setprecision(4);cout<<"POLYHEDRON GEOMETRY TOOL (C++ demo)\n";cout<<"Supported faces: 4 tetrahedron, 6 cube, 8 octahedron, 12 dodecahedron, 20 icosahedron\n";
 while(true){cout<<"\nEnter face count (0 to exit): ";int n;if(!(cin>>n))break;if(n==0)break;cout<<"Enter edge length in cm: ";double e;if(!(cin>>e))break;if(!isfinite(e)||e<=0){cout<<"Error: edge length must be positive.\n";continue;}
 try{Solid s=regular(n,e);Stats t=stats(s);cout<<"Vertices: "<<t.vertices<<" | Edges: "<<t.edges<<" | Faces: "<<t.faces<<"\nSurface area: "<<t.area<<" cm^2 | Volume: "<<t.volume<<" cm^3\n";
 cout<<"Cut with plane nx*x + ny*y + nz*z = d? (1=yes, 0=no): ";int yes;if(!(cin>>yes))break;if(yes==1){V normal;double d;cout<<"Enter nx ny nz d (example: 1 0 0 0): ";if(!(cin>>normal.x>>normal.y>>normal.z>>d))break;Cut result=cut(s,normal,d);if(!result.valid)cout<<"No valid cut: plane does not divide solid into two positive-volume pieces.\n";else {Stats a=stats(result.a),b=stats(result.b);cout<<"Cut section area: "<<result.sectionArea<<" cm^2\n";cout<<"Piece 1 volume: "<<a.volume<<" cm^3; surface area: "<<a.area<<" cm^2\n";cout<<"Piece 2 volume: "<<b.volume<<" cm^3; surface area: "<<b.area<<" cm^2\n";cout<<"Volume conservation error: "<<abs(a.volume+b.volume-t.volume)<<" cm^3\n";}}
 }catch(const exception& ex){cout<<"Error: "<<ex.what()<<"\n";}
 }
 cout<<"Goodbye!\n";
}
