
#include "delaunator.h"
#include <cstdio>
#include <limits>
#include <tuple>
#include <exception>
#include <cmath>
#include "prettyprint.hpp"
#include <iostream>

using namespace std;

namespace {
    const double max_double = numeric_limits<double>::max();
    double dist(
        const double &ax,
        const double &ay,
        const double &bx,
        const double &by
    ) {
        const double dx = ax - bx;
        const double dy = ay - by;
        return dx * dx + dy * dy;
    }
    double circumradius(
        const double &ax,
        const double &ay,
        const double &bx,
        const double &by,
        const double &cx,
        const double &cy
    ) {
        const double dx = bx - ax;
        const double dy = by - ay;
        const double ex = cx - ax;
        const double ey = cy - ay;

        const double bl = dx * dx + dy * dy;
        const double cl = ex * ex + ey * ey;
        const double d = dx * ey - dy * ex;

        const double x = (ey * bl - dy * cl) * 0.5 / d;
        const double y = (dx * cl - ex * bl) * 0.5 / d;

        if (bl && cl && d) {
            return x * x + y * y;
        } else {
            return max_double;
        }
    }

    double area(
        const double px,
        const double py,
        const double qx,
        const double qy,
        const double rx,
        const double ry
    ) {
        return (qy - py) * (rx - qx) - (qx - px) * (ry - qy);
    }

    tuple<double, double> circumcenter(
        const double &ax,
        const double &ay,
        const double &bx,
        const double &by,
        const double &cx,
        const double &cy
    ) {
        const double dx = bx - ax;
        const double dy = by - ay;
        const double ex = cx - ax;
        const double ey = cy - ay;

        const double bl = dx * dx + dy * dy;
        const double cl = ex * ex + ey * ey;
        const double d = dx * ey - dy * ex;

        const double x = ax + (ey * bl - dy * cl) * 0.5 / d;
        const double y = ay + (dx * cl - ex * bl) * 0.5 / d;

        return make_tuple(x, y);
    }
    double compare(
        const vector<double> &coords,
        unsigned long int i,
        unsigned long int j,
        double cx,
        double cy
    ) {
        const double d1 = dist(coords[2 * i], coords[2 * i + 1], cx, cy);
        const double d2 = dist(coords[2 * j], coords[2 * j + 1], cx, cy);
        const double diff1 = d1 - d2;
        const double diff2 = coords[2 * i] - coords[2 * j];
        const double diff3 = coords[2 * i + 1] - coords[2 * j + 1];

        if (diff1) {
            return diff1;
        } else if(diff2) {
            return diff2;
        } else {
            return diff3;
        }
    }
    // std::deque<long int>::iterator insert_node(
    //     const deque<DelaunatorPoint> &hull,
    //     const vector<double> &coords,
    //     long int i,
    //     std::deque<long int>::iterator prev
    // ) {
    //     return hull.emplace(prev + 1, {
    //         .i = i,
    //         .x = coords[2 * i],
    //         .y = coords[2 * i + 1],
    //         .t = 0
    //     });
    // }
    // size_t insert_node(
    //     deque<DelaunatorPoint> &hull,
    //     const vector<double> &coords,
    //     long int i
    // ) {
    //     DelaunatorPoint p = {
    //         .i = i,
    //         .x = coords[2 * i],
    //         .y = coords[2 * i + 1],
    //         .t = 0
    //     };
    //     size_t pos = hull.insert(hull.end(), p) - hull.begin();
    //     return pos;
    // }
    void quicksort(
        unsigned long int ids[],
        const vector<double> &coords,
        unsigned int left,
        unsigned int right,
        double &cx,
        double &cy
    ) {
        long int i;
        long int j;
        unsigned long int temp;

        if (right - left <= 20) {
            for (i = left + 1; i <= right; i++) {
                // printf("i=%lu\n", i);
                temp = ids[i];
                j = i - 1;
                while (
                    j >= left &&
                    compare(coords, ids[j], temp, cx, cy) > 0
                ) {
                    // printf("j=%lu\n", j);
                    ids[j + 1] = ids[j];
                    j--;
                }
                ids[j + 1] = temp;
            }
        } else {
            throw runtime_error("not implemented");
        }// else {
    //         const median = (left + right) >> 1;
    //         i = left + 1;
    //         j = right;
    //         swap(ids, median, i);
    //         if (compare(coords, ids[left], ids[right], cx, cy) > 0) swap(ids, left, right);
    //         if (compare(coords, ids[i], ids[right], cx, cy) > 0) swap(ids, i, right);
    //         if (compare(coords, ids[left], ids[i], cx, cy) > 0) swap(ids, left, i);

    //         temp = ids[i];
    //         while (true) {
    //             do i++; while (compare(coords, ids[i], temp, cx, cy) < 0);
    //             do j--; while (compare(coords, ids[j], temp, cx, cy) > 0);
    //             if (j < i) break;
    //             swap(ids, i, j);
    //         }
    //         ids[left + 1] = ids[j];
    //         ids[j] = temp;

    //         if (right - i + 1 >= j - left) {
    //             quicksort(ids, coords, i, right, cx, cy);
    //             quicksort(ids, coords, left, j - 1, cx, cy);
    //         } else {
    //             quicksort(ids, coords, left, j - 1, cx, cy);
    //             quicksort(ids, coords, i, right, cx, cy);
    //         }
    //     }
    }

    bool in_circle(
        double ax, double ay,
        double bx, double by,
        double cx, double cy,
        double px, double py
    ) {
        const double dx = ax - px;
        const double dy = ay - py;
        const double ex = bx - px;
        const double ey = by - py;
        const double fx = cx - px;
        const double fy = cy - py;

        const double ap = dx * dx + dy * dy;
        const double bp = ex * ex + ey * ey;
        const double cp = fx * fx + fy * fy;

        return (dx * (ey * cp - bp * fy) -
            dy * (ex * cp - bp * fx) +
            ap * (ex * fy - ey * fx)) < 0;
    }
}

Delaunator::Delaunator(const vector<double> &in_coords) {
    m_coords = move(in_coords);
    const long int n = m_coords.size() >> 1;
    double max_x = -1 * max_double;
    double max_y = -1 * max_double;
    double min_x = max_double;
    double min_y = max_double;
    unsigned long int ids[n];
    for (long int i = 0; i < n; i++) {
        const double x = m_coords[2 * i];
        const double y = m_coords[2 * i + 1];
        // printf("%f %f", x, y);

        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;
        ids[i] = i;
    }
    const double cx = (min_x + max_x) / 2;
    const double cy = (min_y + max_y) / 2;
    double min_dist = max_double;
    unsigned long int i0;
    unsigned long int i1;
    unsigned long int i2;

    // pick a seed point close to the centroid
    for (unsigned long int i = 0; i < n; i++) {
        const double d = dist(cx, cy, m_coords[2 * i], m_coords[2 * i + 1]);
        if (d < min_dist) {
            i0 = i;
            min_dist = d;
        }
    }

    min_dist = max_double;

    // find the point closest to the seed
    for (unsigned long int i = 0; i < n; i++) {
        if (i == i0) continue;
        const double d = dist(m_coords[2 * i0], m_coords[2 * i0 + 1], m_coords[2 * i], m_coords[2 * i + 1]);
        if (d < min_dist && d > 0)
        {
            i1 = i;
            min_dist = d;
        }
    }

    double min_radius = max_double;

    // find the third point which forms the smallest circumcircle with the first two
    for (unsigned long int i = 0; i < n; i++)
    {
        if (i == i0 || i == i1) continue;

        const double r = circumradius(
            m_coords[2 * i0], m_coords[2 * i0 + 1],
            m_coords[2 * i1], m_coords[2 * i1 + 1],
            m_coords[2 * i], m_coords[2 * i + 1]);

        if (r < min_radius)
        {
            i2 = i;
            min_radius = r;
        }
    }

    if (min_radius == max_double) {
        throw runtime_error("not triangulation");;
    }

    if (
        area(
            m_coords[2 * i0], m_coords[2 * i0 + 1],
            m_coords[2 * i1], m_coords[2 * i1 + 1],
            m_coords[2 * i2], m_coords[2 * i2 + 1]
        ) < 0
    ) {
        const double tmp = i1;
        i1 = i2;
        i2 = tmp;
    }

    const double i0x = m_coords[2 * i0];
    const double i0y = m_coords[2 * i0 + 1];
    const double i1x = m_coords[2 * i1];
    const double i1y = m_coords[2 * i1 + 1];
    const double i2x = m_coords[2 * i2];
    const double i2y = m_coords[2 * i2 + 1];

    tie(m_center_x, m_center_y) = circumcenter(i0x, i0y, i1x, i1y, i2x, i2y);

    // sort the points by distance from the seed triangle circumcenter
    quicksort(ids, m_coords, 0, n - 1, m_center_x, m_center_y);

    m_hash_size = ceil(sqrt(n));
    m_hash.reserve(m_hash_size);
    for (int i = 0; i < m_hash_size; i++) m_hash.push_back(-1);

    m_hull.reserve(m_coords.size());

    long int e = insert_node(i0);
    hash_edge(e);
    m_hull[e].t = 0;

    e = insert_node(i1, e);
    hash_edge(e);
    m_hull[e].t = 1;

    e = insert_node(i2, e);
    hash_edge(e);
    m_hull[e].t = 2;

    cout << "m_hash " << m_hash << endl;

    const size_t max_triangles = 2 * n - 5;
    triangles.reserve(max_triangles * 3);
    halfedges.reserve(max_triangles * 3);
    add_triangle(i0, i1, i2, -1, -1, -1);

    double xp = NAN;
    double yp = NAN;
    for (size_t k = 0; k < n; k++) {
        const size_t i = ids[k];
        const double x = m_coords[2 * i];
        const double y = m_coords[2 * i + 1];
        if (x == xp && y == yp) continue;
        xp = x;
        yp = y;
        if (
                (x == i0x && y == i0y) ||
                (x == i1x && y == i1y) ||
                (x == i2x && y == i2y)
        ) continue;

        const size_t start_key = hash_key(x, y);
        size_t key = start_key;
        long int start = -1;
        do {
            start = m_hash[key];
            key = (key + 1) % m_hash_size;
        } while(
            (start < 0 || m_hull[start].removed) &&
            (key != start_key)
        );

        e = start;

        while(
            area(
                x, y,
                m_hull[e].x, m_hull[e].y,
                m_hull[m_hull[e].next].x, m_hull[m_hull[e].next].y
            ) >= 0
        ) {
            if (m_hull[e].next == -1) {
                cout << e << endl;
            }

            e = m_hull[e].next;

            if (e == start) {
                throw runtime_error("Something is wrong with the input points.");
            }
        }

        const bool walk_back = e == start;

        // add the first triangle from the point
        size_t t = add_triangle(
            m_hull[e].i,
            i,
            m_hull[m_hull[e].next].i,
            -1, -1, m_hull[e].t
        );

        m_hull[e].t = t; // keep track of boundary triangles on the hull
        e = insert_node(i, e);

        // recursively flip triangles from the point until they satisfy the Delaunay condition
        m_hull[e].t = legalize(t + 2);

        if (m_hull[m_hull[m_hull[e].prev].prev].t == halfedges[t + 1]) {
            m_hull[m_hull[m_hull[e].prev].prev].t = t + 2;
        }

        // walk forward through the hull, adding more triangles and flipping recursively
        long int q = m_hull[e].next;
        while(
            area(
                x, y,
                m_hull[q].x, m_hull[q].y,
                m_hull[m_hull[q].next].x, m_hull[m_hull[q].next].y
            ) < 0
        ) {
            t = add_triangle(
                m_hull[q].i, i,
                m_hull[m_hull[q].next].i, m_hull[m_hull[q].prev].t,
                -1, m_hull[q].t
            );
            m_hull[m_hull[q].prev].t = legalize(t + 2);
            remove_node(q);
            q = m_hull[q].next;
        }

        if (walk_back) {
            // walk backward from the other side, adding more triangles and flipping
            q = m_hull[e].prev;
            while(
                area(
                    x, y,
                    m_hull[m_hull[q].prev].x, m_hull[m_hull[q].prev].y,
                    m_hull[q].x, m_hull[q].y
                ) < 0
            ) {
                t = add_triangle(
                    m_hull[m_hull[q].prev].i, i,
                    m_hull[q].i, -1,
                    m_hull[q].t, m_hull[m_hull[q].prev].t
                );
                legalize(t + 2);
                m_hull[m_hull[q].prev].t = t;
                remove_node(q);
                q = m_hull[q].prev;
            }
        }

        hash_edge(e);
        hash_edge(m_hull[e].prev);
    }

};

size_t Delaunator::remove_node(size_t node) {
    m_hull[m_hull[node].prev].next = m_hull[node].next;
    m_hull[m_hull[node].next].prev = m_hull[node].prev;
    m_hull[node].removed = true;
    return m_hull[node].prev;
}

size_t Delaunator::legalize(size_t a) {
    size_t halfedges_size = halfedges.size();
    const long int b = halfedges[a];

    const long int a0 = a - a % 3;
    const long int b0 = b - b % 3;

    const long int al = a0 + (a + 1) % 3;
    const long int ar = a0 + (a + 2) % 3;
    const long int bl = b0 + (b + 2) % 3;

    const long int p0 = triangles[ar];
    const long int pr = triangles[a];
    const long int pl = triangles[al];
    const long int p1 = triangles[bl];

    const bool illegal = in_circle(
            m_coords[2 * p0], m_coords[2 * p0 + 1],
            m_coords[2 * pr], m_coords[2 * pr + 1],
            m_coords[2 * pl], m_coords[2 * pl + 1],
            m_coords[2 * p1], m_coords[2 * p1 + 1]
    );

    if (illegal) {
        triangles[a] = p1;
        triangles[b] = p0;
        link(a, halfedges[bl]);
        link(b, halfedges[ar]);
        link(ar, bl);

        const long int br = b0 + (b + 1) % 3;

        legalize(a);
        return legalize(br);
    }

}

size_t Delaunator::insert_node(size_t i, size_t prev) {
    const size_t node = insert_node(i);
    m_hull[node].next = m_hull[prev].next;
    m_hull[node].prev = prev;
    m_hull[m_hull[node].next].prev = node;
    m_hull[prev].next = node;
    return node;
};

size_t Delaunator::insert_node(size_t i) {
    long int node = m_hull.size();
    DelaunatorPoint p = {
        .i = i,
        .x = m_coords[2 * i],
        .y = m_coords[2 * i + 1],
        .prev = node,
        .next = node,
        .removed = false
    };
    m_hull.push_back(move(p));
    return node;
}

size_t Delaunator::hash_key(double x, double y) {
    const double dx = x - m_center_x;
    const double dy = y - m_center_y;
    // use pseudo-angle: a measure that monotonically increases
    // with real angle, but doesn't require expensive trigonometry
    const double p = 1 - dx / (abs(dx) + abs(dy));
    return floor((2 + (dy < 0 ? -p : p)) / 4 * m_hash_size);
}

void Delaunator::hash_edge(size_t e){
    m_hash[hash_key(m_hull[e].x, m_hull[e].y)] = e;
}

size_t Delaunator::add_triangle(
    size_t i0, size_t i1, size_t i2,
    size_t a, size_t b, size_t c
) {
    const size_t t = triangles.size();
    triangles.push_back(i0);
    triangles.push_back(i1);
    triangles.push_back(i2);
    link(t, a);
    link(t + 1, b);
    link(t + 2, c);
    return t;
}

void Delaunator::link(size_t a, size_t b) {
    size_t s  = halfedges.size();
    if (a == s) {
        halfedges.push_back(a);
    } else if (a < s) {
        halfedges[a] = b;
    } else {
        throw runtime_error("Cannot link edge");
    }

    if (b > -1) {
        if (b < halfedges.size()) {
            halfedges[b] = a;
        }  else {
            throw runtime_error("Cannot link edge");
        }
    }
};