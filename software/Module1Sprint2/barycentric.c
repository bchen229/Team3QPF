#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Structures.h"

/*
 * GPS coordinates that correspond to the corners of the screen
 */
static double extents[][2] = { 
	{ 49.262161, -123.250812 }, 
	{ 49.263114, -123.248334 }, 
	{ 49.261765, -123.247403 }, 
	{ 49.260998, -123.249801 } 
};

/*
 * Triangulation: a list of indices to vertices
 */
static int TRI[][3] = { 
	{ 2, 3, 4 }, 
	{ 2, 4, 1 } 
};

/*
 * The pixel values for the corners of the screen
 */
static double screen[][2] = { 
	{   0, 	0 }, 
	{ 800,	0 }, 
	{ 800,	480 }, 
	{   0,	480 } 
};

#if !defined(Point2D)
typedef struct {
	double x, y;
} Point2D;

#endif

/*
	Vector3 for holding barycentric coordinates 
*/
typedef struct {
	double x, y, z;
} Vector3;

/*
	Vertex: Differs from Point2D in that it stores values to be interpolated
	using barycentric coordinates
*/
typedef struct {
	Point2D bary;
	double x, y;
} Vertex;

/*
	Triangle: was supposed to cache values to optimize computing barycentric 
	coordinates, however the function did not output correct results
*/
typedef struct {
	// Vertex *v1,*v2,*v3;
	Vertex v1, v2, v3;
	double bary_normalize, dot00, dot01, dot11;
	Point2D e0, e1, centroid;
} Triangle;

/*
	Delaunay: structure to hold the triangulation connectivity
	contains the vertices, triangles and centroids for point location
*/
typedef struct {
	int numv, numt;
	Vertex* vertices;
	Triangle* T;
	Point2D *centroids;
} Delaunay;

/*
	Constructor for Vector3: completely defines the fields
*/
Vector3 vec3(double x, double y, double z) {
	Vector3 v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}



/*
	Dot product of two vertices
*/
double dotv(Vertex v1, Vertex v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

/*
	Dot product of two points
*/
double dotp(Point2D p1, Point2D p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

/*
	Dot product for 3 components
*/
double dot3(Vector3 v1, Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
	Cross product of two points
*/
double crossp(Point2D p1, Point2D p2) {
	return p1.x * p2.y - p2.x * p1.y;
}

/*
	Cross product of two vertices
*/
double crossv(Vertex v1, Vertex v2) {
	return v1.x * v2.y - v2.x * v1.y;
}

/*
	Subract two vertices yielding a vertex
*/
Vertex vsubv(Vertex v1, Vertex v2) {
	Vertex v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	return v;
}

/*
	Subract two vertices yielding a point
*/
Point2D vsubp(Vertex v1, Vertex v2) {
	Point2D p;
	p.x = v1.x - v2.x;
	p.y = v1.y - v2.y;
	return p;
}

/*
	Subract two points yielding a point
*/
Point2D psubp(Point2D p1, Point2D p2) {
	Point2D p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}

/*
	Compute the squared norm of a vertex
*/
double normsqv(Vertex v) {
	return dotv(v, v);
}

/*
	Compute the squared norm of a point
*/
double normsqp(Point2D p) {
	return dotp(p, p);
}

double normp(Point2D p) {
	return sqrt(dotp(p, p));
}

/*
	Compute the 2-norm of a point
*/
Point2D point(double x, double y) {
	Point2D p;
	p.x = x;
	p.y = y;
	return p;
}




/*
	Constructor for a vertex
*/
Vertex vtx(double x, double y, Point2D b) {
	Vertex v;
	v.x = x;
	v.y = y;
	v.bary = b;
	return v;
}

/*
	Converts a Vertex into a Point2D
*/
Point2D v2p(Vertex v) {
	return point(v.x, v.y);
}

/*
	Overloaded Vertex Constructor
*/
Vertex Vert(double x, double y, double bx, double by) {
	Vertex v;
	v.x = x;
	v.y = y;
	v.bary.x = bx;
	v.bary.y = by;

	return v;
}

/*
	Computes the average of 3 vertices of a triangle
*/
Point2D computeCentroid(Triangle T) {
	double xavg = (T.v1.x + T.v2.x + T.v3.x) / 3;
	double yavg = (T.v1.y + T.v2.y + T.v3.y) / 3;
	// double zavg = (T.v1->z + T.v2->z + T.v3->z)/3;

	return point(xavg, yavg);
}

/*
	Triangle Constructor: pre-caches values needed to compute the
	barycentric coordinates. Unfotunately the optimized code did not work.
*/
Triangle makeTri(Vertex v1, Vertex v2, Vertex v3) {
	Triangle T;
	T.v1 = v1;
	T.v2 = v2;
	T.v3 = v3;

	T.e0 = vsubp(v2, v1);
	T.e1 = vsubp(v3, v1);

	T.dot00 = dotp(T.e0, T.e0);
	T.dot01 = dotp(T.e0, T.e1);
	T.dot11 = dotp(T.e1, T.e1);

	T.bary_normalize = 1 / (T.dot00 * T.dot01 - T.dot01 * T.dot01);

	T.centroid = computeCentroid(T);
	return T;
}

/*
	An attempt to reduce the computational overhead 
	(however, since this isn't 3D graphics, this function
	would be called relatively infrequently in our usage)

	this function referenced from
	http://gamedev.stackexchange.com/a/23745
*/
Vector3 barycentric(Triangle T, Point2D q) {
	// Point e2 = psubp(q, v2p(*(T.v1)));
	Point2D e2 = psubp(q, v2p(T.v1));

	// computing intermediate values for 2d cross product
	// using values cached in Triangle objects
	double dot20 = dotp(e2, T.e0);
	double dot21 = dotp(e2, T.e1);

	// compute the coefficients
	// Note that the normalization factor is stored already inverted
	// to reduce division operations.
	double v = (T.dot11 * dot20 - T.dot01 * dot21) * T.bary_normalize;
	double w = (T.dot00 * dot21 - T.dot01 * dot20) * T.bary_normalize;
	// reduce computation by exploiting knowledge that these sum to 1
	double u = 1.f - v - w;

	return vec3(u, v, w);
}

/*
	Alternative implementation of barycentric computation.
	Standard mathematical derivation.
	Tested to reflect results in MATLAB
*/
Vector3 barycentric2(Triangle T, Point2D q) {
	// Compute the area of the triangle
	double area = .5 * (crossp(vsubp(T.v2, T.v1), vsubp(T.v3, T.v1)));

	// Compute the areas of 2 out of 3 sub-triangles induced by the query point q
	double area23q = .5 * (crossp(psubp(v2p(T.v2), q), psubp(v2p(T.v3), q)));
	double area13q = .5 * (crossp(psubp(v2p(T.v3), q), psubp(v2p(T.v1), q)));

	// Normalize the coefficients using the area of entire triangle
	// Should be clear geometrically that bx+by+bz = 1
	double bx = area13q / area;
	double bz = area23q / area;
	// reduce computation by exploiting knowledge that these sum to 1
	double by = 1.0 - bx - bz;

	return vec3(bx, by, bz);
}

/*
	Centroid comparator (Unused)
*/
int centroidLessThan(Triangle t1, Triangle t2, Point2D p) {
	double dist1 = normsqp(psubp(t1.centroid, p));
	double dist2 = normsqp(psubp(t2.centroid, p));

	return (dist1 < dist2) ? 1 : 0;
}

/*
	Centroid comparator (Unused)
*/
int centroidGreaterThan(Triangle t1, Triangle t2, Point2D p) {
	double dist1 = normsqp(psubp(t1.centroid, p));
	double dist2 = normsqp(psubp(t2.centroid, p));

	return (dist1 > dist2) ? 1 : 0;
}

/*
	Centroid comparator (Unused)
*/
int centroidEqualTo(Triangle t1, Triangle t2, Point2D p, double eps) {
	double dist1 = normsqp(psubp(t1.centroid, p));
	double dist2 = normsqp(psubp(t2.centroid, p));

	return (fabs(dist1 - dist2) < eps) ? 1 : 0;
}

/*
	Finds the index to the minimum (or first minima) of an array
	For use in nearest neighbour determination.
*/
int findMin(double *numbers, int size) {
	// TODO: should check whether list is empty
	int i, min = numbers[0];
	for (i = 1; i < size; i++)
		(numbers[i] < numbers[min]) ? min = i : 1;
	return min;
}

/*
	Returns the minimum (or first minima) of an array
*/
double showMin(double *numbers, int size) {
	// TODO: should check whether list is empty
	int i, min = 1e6;
	for (i = 0; i < size; i++)
		(numbers[i] < min) ? min = numbers[i] : 1;
	return min;
}

/*
	Given a triangulation d and a query point q,
	finds the nearest neighbour to the point q using the precomputed
	centroids. A more sophisticated relational data structure could be 
	implemented over top of this simple function. 

	Simplicity was desired for the NIOS
*/
Triangle nearestneighbour(Delaunay d, Point2D q) {
	int i;
	// printf("test: %.10f %.10f\n", q.x,q.y);
	// instantiate list of numbers size equal to number of triangles
	double neighbours[d.numt];
	// iterate through the centroids stored in triangulation object
		// |centroids| = |triangles|
	for (i = 0; i < d.numt; i++) {
		// printf("centroid: %.10f %.10f\n", d.T[i].centroid.x,d.T[i].centroid.y);
		neighbours[i] = normp(psubp(d.T[i].centroid, q));	// Use a standard distance metric and store
		// printf("neighbours[i]: %.10f\n", neighbours[i]);
	}

	// printf("%d\n", findMin(neighbours,d.numt));

	// return index to the min distance
	// want the index so that we can later find the triangle
	return d.T[findMin(neighbours, d.numt)];
}


/*
	Define a Delaunay Triangulation.
	*** Loads connectivity information from MATLAB computation

*/
Delaunay DelaunayTriangulation(double vs[][2], double bary[][2], int tris[][3],
		int vsize, int bsize, int tsize) {
	// we need as many interpolation values to store in the vertices as 
	// the number of vertices
	assert(vsize == bsize);
	
	Delaunay d;
	Triangle tri;
	int i;

	// define sizes
	d.numv = vsize;
	d.numt = tsize;
	// initialize arrays
	d.vertices = malloc(sizeof(Vertex) * vsize);
	d.centroids = malloc(sizeof(int) * tsize);
	d.T = malloc(sizeof(Triangle) * tsize);

	// construct all the vertices (copies)
	for (i = 0; i < vsize; i++) {
		d.vertices[i] = vtx(vs[i][0], vs[i][1], point(bary[i][0], bary[i][1]));
		// printf("%f %f\n", vs[i][0],vs[i][1]);
	}

	// easy indices
	int one, two, three;
	// next construct the triangles
	for (i = 0; i < tsize; i++) {
		// indices need to be converted from MATLAB
		// 1-indexing to C 0-indexing
		one = tris[i][0] - 1;
		two = tris[i][1] - 1;
		three = tris[i][2] - 1;

		// memory usage is a bit lavish ...for peace of mind
		Vertex v1 = vtx(vs[one][0], vs[one][1],
				point(bary[one][0], bary[one][1]));
		Vertex v2 = vtx(vs[two][0], vs[two][1],
				point(bary[two][0], bary[two][1]));
		Vertex v3 = vtx(vs[three][0], vs[three][1],
				point(bary[three][0], bary[three][1]));

		// precompute and define variables
		tri = makeTri(v1, v2, v3);

		// add to triangulation
		d.T[i] = tri;
		// don't know where we might need the centroid
		// maybe not necessary since the same index is used
		d.centroids[i] = (tri.centroid);
	}

	return d;
}

/*
	Delaunay "copy constructor"
*/
Delaunay CopyDelaunayTriangulation(Vertex* vs, Triangle* ts, int vsize,
		int tsize) {
	Delaunay d;
	int i;
	d.vertices = malloc(sizeof(Vertex) * vsize);
	d.T = malloc(sizeof(Triangle) * tsize);
	for (i = 0; i < vsize; i++)
		d.vertices[i] = vs[i];
	for (i = 0; i < tsize; i++)
		d.T[i] = ts[i];

	return d;
}



/*
	Returns a screen (x,y) coordinate given a barycentric
	coordinate component vector b. This uses the values stored
	in each vertex to interpolate.

	*** Had to hack the ordering to match MATLAB results
*/
Point2D toScreen(Triangle tt, Vector3 b) {
	// computes the dot product for the linear combination of the vertex values
	// results in a screen point
	return point(dot3(b, vec3(tt.v2.bary.x, tt.v3.bary.x, tt.v1.bary.x)),
			dot3(b, vec3(tt.v2.bary.y, tt.v3.bary.y, tt.v1.bary.y)));
}

/*
	Wrapper function
	Computes a (x,y) pixel given GPS lattitude and longitude values
*/
void computePixel(double lat, double lon, int * latPix, int * lonPix){
	// load the triangulation from MATLAB
	Delaunay del_all = DelaunayTriangulation(extents,screen,TRI,4,4,2);
	// set GPS query point
	Point2D q = point(lat,lon);
	// find nearest triangle so q is in its interior
	Triangle tt = nearestneighbour(del_all,q);
	// compute the coefficients 
	Vector3 vvv = barycentric2(tt,q);
	// find the barycentric interpolation 
	Point2D pixel = toScreen(tt,vvv);
	printf("%f %f\n", pixel.x, pixel.y);

	// submit the referenced pixel values
	*latPix = (int) pixel.x;
	*lonPix = (int) pixel.y;

	printf("Lat: %d, Long %d \n", *latPix, *lonPix);
}

