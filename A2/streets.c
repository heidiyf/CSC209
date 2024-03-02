#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "streets.h"
#include <float.h> 

struct node {
    double lat;
    double lon;
    int num_ways;
    int *num_ids;
    /* TODO: task 1 */
};

struct way {
    /* TODO: task 1 */
    int id;
    char *name;
    float maxspeed;
    bool oneway;
    int num_nodes;
    int *node_ids;
};

struct ssmap {
    /* TODO: task 1 */
    struct node **nodes; // Pointer to an array of node pointers
    struct way **ways;    //                     of way pointers
    int nr_ways;
    int nr_nodes;
};

typedef struct {
    int node_id;
    double distance; // Priority based on distance
} NodePriority;

typedef struct {
    NodePriority *elements;
    int size;
    int capacity;
} PriorityQueue;

struct ssmap * 
ssmap_create(int nr_nodes, int nr_ways)
{
    /* TODO: task 2 */

    // if either paramerer is 0
    if (nr_nodes == 0 || nr_ways == 0) {
        return NULL;
    }

    // Allocate memory for the ssmap structure 
    struct ssmap *m = malloc(sizeof(struct ssmap));

    // out of memory case
    if (m == NULL) {
        return NULL;
    }

    // load notes:
    m->nodes = malloc(sizeof(struct node *) * nr_nodes);
    if (m->nodes == NULL) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < nr_nodes; i++) {
        m->nodes[i] = NULL;
    }

    // load ways:
    m->ways = malloc(sizeof(struct way *) * nr_ways);
    if (m->ways == NULL) {
        free(m->nodes);
        free(m);
        return NULL;
    }

    for (int i = 0; i < nr_ways; i++) {
        m->ways[i] = NULL;
    }

    // load nr_nodes
     m->nr_nodes = nr_nodes;
     m->nr_ways = nr_ways;

    return m;
}

bool
ssmap_initialize(struct ssmap * m)
{
    /* TODO: task 2
     * additional initialization code can be added here */
    return true;
}

void
ssmap_destroy(struct ssmap * m)
{
    /* TODO: task 2 */
    if (m != NULL) {
        // Free nodes
        if (m->nodes != NULL) {
            for (int i = 0; i < m->nr_nodes; i++) {
                if (m->nodes[i] != NULL) {
                    free(m->nodes[i]->num_ids); // Free the node's way IDs
                    free(m->nodes[i]);          // Free the node itself
                }
            }
            free(m->nodes); // Free the array of node pointers
        }

        // Free ways
        if (m->ways != NULL) {
            for (int j = 0; j < m->nr_ways; j++) {
                if (m->ways[j] != NULL) {
                    free(m->ways[j]->name);      // Free the way's name
                    free(m->ways[j]->node_ids);  // Free the way's node IDs
                    free(m->ways[j]);            // Free the way itself
                }
            }
            free(m->ways); // Free the array of way pointers
        }

        free(m); // Finally, free the ssmap structure itself
    }
}

struct way * 
ssmap_add_way(struct ssmap * m, int id, const char * name, float maxspeed, bool oneway, 
              int num_nodes, const int node_ids[num_nodes])
{
    /* TODO: task 2 */

    // check valid index
    if (id < 0 || id >= m->nr_ways || m->ways[id] != NULL) {
        return NULL; 
    }

    struct way *new_way = malloc(sizeof(struct way));
    if (new_way == NULL) {
        return NULL;
    }

    //node id
    new_way->node_ids = malloc(sizeof(int) * num_nodes );
    if (new_way->node_ids == NULL) {
        free(new_way); 
        return NULL;
    }
    // Copy each node ID.
    for (int i = 0; i < num_nodes; i++) {
        new_way->node_ids[i] = node_ids[i]; 
    }


    //name
    new_way->name = strdup(name);
    if (new_way->name == NULL) {
        free(new_way->node_ids);
        free(new_way); 
        return NULL;
    }

    new_way->id = id;
    new_way->maxspeed = maxspeed;
    new_way->oneway = oneway;
    new_way->num_nodes = num_nodes;

    m->ways[id] = new_way;

    return new_way;
}

struct node * 
ssmap_add_node(struct ssmap * m, int id, double lat, double lon, 
               int num_ways, const int way_ids[num_ways])
{
    /* TODO: task 2 */
    if (id < 0 || id >= m->nr_nodes || m->nodes[id] != NULL){
        return NULL;
    }

    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL){
        return NULL;
    }

    //num (way) id
    if (num_ways > 0){
        new_node->num_ids = malloc(num_ways * sizeof(int));
        if (new_node->num_ids == NULL){
            free(new_node);
            return NULL;
        }
        for (int i = 0; i < num_ways; i++) {
            new_node ->num_ids[i] = way_ids[i];
        }
    }else { // num ways <= 0
        new_node->num_ids = NULL;
    }

    new_node->lat = lat;
    new_node->lon = lon;
    new_node->num_ways = num_ways;

    m->nodes[id] = new_node;

    return new_node;
}

void
ssmap_print_way(const struct ssmap * m, int id)
{
    /* TODO: task 3 */

    //check if id is invalid
    if(id < 0 || id >= m->nr_ways || m->ways[id]==NULL){
        printf("error: way %d does not exist.\n", id);
        return;
    }
    struct way *way = m->ways[id];
    //first line
    printf("Way %d: %s\n", id, way->name);
    //optional
    printf("Max Speed: %.2f km/h\n", way->maxspeed);
    printf("One-way: %s\n", way->oneway ? "Yes" : "No");
    printf("Number of Nodes on this way: %d\n", way->num_nodes);
    if (way->num_nodes>0){
        printf("Node IDs on this way:");
        for (int i = 0; i < way->num_nodes; i++){
            printf("%d", way->node_ids[i]);
            if (i < way->num_nodes - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void
ssmap_print_node(const struct ssmap * m, int id)
{
    /* TODO: task 3 */
    if (id < 0 || id >= m->nr_nodes || m->nodes[id] == NULL){
        printf("error: node %d does not exist\n", id);
        return;
    }
    struct node *node = m->nodes[id];
    printf("Node %d: (%.7f, %.7f)\n", id, node->lat, node->lon);

    printf("Number of Ways associated with this node: %d\n", node->num_ways);
    if (node->num_ways > 0 && node->num_ids != NULL) {
        printf("Way IDs cassociated with this node: ");
        for (int i = 0; i < node->num_ways; i++) {
            printf("%d", node->num_ids[i]);
            if (i < node->num_ways - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void 
ssmap_find_way_by_name(const struct ssmap * m, const char * name)
{
    /* TODO: task 3 */
    for (int i = 0; i < m->nr_ways; i++) {
        struct way *way = m->ways[i];
        if (way != NULL && strstr(way->name, name) != NULL) {
            printf("%d ", way->id);
        }
    }
    printf("\n"); // Ensure output ends with a newline for clean formatting.
}


void 
ssmap_find_node_by_names(const struct ssmap * m, const char * name1, const char * name2)
{
    bool isSameName = name2 && strcmp(name1, name2) == 0; // Check if name1 and name2 are identical

    for (int i = 0; i < m->nr_nodes; i++) {
        struct node *node = m->nodes[i];
        if (node == NULL) continue; // Skip NULL nodes

        bool matchesName1 = false;
        bool matchesName2 = false;
        int wayMatchForName1 = -1; // Store way ID that matches name1 (initially invalid)
        int wayMatchForName2 = -1; // Store way ID that matches name2 (initially invalid)

        for (int j = 0; j < node->num_ways; j++) {
            struct way *way = m->ways[node->num_ids[j]];
            if (way == NULL) continue; // Skip NULL ways

            // Check if way's name contains name1
            if (strstr(way->name, name1) != NULL) {
                if (!matchesName1) {
                    matchesName1 = true;
                    wayMatchForName1 = way->id; // Store first matching way ID for name1
                } else if (isSameName && wayMatchForName1 != way->id) {
                    matchesName2 = true; // Found a second distinct match for the same name
                    wayMatchForName2 = way->id; // Store second matching way ID
                    break; // Found both matches, no need to continue
                }
            }
            // If name1 and name2 are different, check for name2 match
            if (!isSameName && name2 && strstr(way->name, name2) != NULL && wayMatchForName1 != way->id) {
                matchesName2 = true;
                wayMatchForName2 = way->id; // Store matching way ID for name2
                // If matches for both names are found, break early
                if (matchesName1) {
                    break;
                }
            }
        }

        // Print condition for when name1 == name2
        if (isSameName && matchesName1 && matchesName2 && wayMatchForName1 != wayMatchForName2) {
            printf("%d ", i);
        }
        // Print condition for when name1 != name2
        else if (!isSameName && matchesName1 && matchesName2) {
            printf("%d ", i);
        }
        // Print condition when name2 is NULL
        else if (!name2 && matchesName1) {
            printf("%d ", i);
        }
    }
    printf("\n"); // Ensure output ends with a newline
}


/**
 * Converts from degree to radian
 *
 * @param deg The angle in degrees.
 * @return the equivalent value in radian
 */
#define d2r(deg) ((deg) * M_PI/180.)

/**
 * Calculates the distance between two nodes using the Haversine formula.
 *
 * @param x The first node.
 * @param y the second node.
 * @return the distance between two nodes, in kilometre.
 */
static double
distance_between_nodes(const struct node * x, const struct node * y) {
    double R = 6371.;       
    double lat1 = x->lat;
    double lon1 = x->lon;
    double lat2 = y->lat;
    double lon2 = y->lon;
    double dlat = d2r(lat2-lat1); 
    double dlon = d2r(lon2-lon1); 
    double a = pow(sin(dlat/2), 2) + cos(d2r(lat1)) * cos(d2r(lat2)) * pow(sin(dlon/2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
    return R * c; 
}

double 
ssmap_path_travel_time(const struct ssmap * m, int size, int node_ids[size])
{
    /* TODO: task 4 */
    // Basic validation for size
    if (size < 2) {
        printf("error: at least two nodes are required to calculate a path.\n");
        return -1.0;
    }

    double total_time = 0.0; // Total travel time in minutes.

    // First pass to check for node existence (Error 1) and repeated nodes (Error 5)
    for (int i = 0; i < size; ++i) {
        int node_id = node_ids[i];

        // Error 1: Validate node existence.
        if (node_id < 0 || node_id >= m->nr_nodes || m->nodes[node_id] == NULL) {
            printf("error: node %d does not exist.\n", node_id);
            return -1.0;
        }

        // Error 5: Check for repeated nodes in the path.
        for (int j = i + 1; j < size; ++j) {
            if (node_ids[j] == node_id) {
                printf("error: node %d appeared more than once.\n", node_id);
                return -1.0;
            }
        }
    }

    // Second pass to check connectivity, adjacency, and one-way compliance (Errors 2, 3, 4)
    for (int i = 0; i < size - 1; ++i) {
        int current_node = node_ids[i];
        int next_node = node_ids[i + 1];

        bool way_found = false;
        for (int w = 0; w < m->nr_ways; ++w) {
            struct way *way = m->ways[w];
            int current_index = -1, next_index = -1;

            for (int n = 0; n < way->num_nodes; ++n) {
                if (way->node_ids[n] == current_node) current_index = n;
                if (way->node_ids[n] == next_node) next_index = n;
            }

            // Error 2: Check if a way exists between current_node and next_node.
            if (current_index != -1 && next_index != -1) {
                way_found = true;

                // Error 3: Check if nodes are adjacent in the way.
                if (abs(current_index - next_index) != 1) {
                    printf("error: cannot go directly from node %d to node %d.\n", current_node, next_node);
                    return -1.0;
                }

                // Error 4: Check for one-way restriction.
                if (way->oneway && current_index > next_index) {
                    printf("error: cannot go in reverse from node %d to node %d.\n", current_node, next_node);
                    return -1.0;
                }

                // Calculate the travel time for this segment.
                double distance = distance_between_nodes(m->nodes[current_node], m->nodes[next_node]);
                total_time += (distance / way->maxspeed) * 60; // Convert travel time to minutes.
                break;
            }
        }

        if (!way_found) {
            printf("error: there are no roads between node %d and node %d.\n", current_node, next_node);
            return -1.0;
        }
    }

    return total_time;
}


// helper fucntion for priority queue
PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->elements = (NodePriority *)malloc(sizeof(NodePriority) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void insertPriorityQueue(PriorityQueue *pq, int node_id, double distance) {
    if (pq->size >= pq->capacity) {
        // Simple resizing for demonstration; real implementation should check for allocation success
        pq->capacity *= 2;
        pq->elements = (NodePriority *)realloc(pq->elements, sizeof(NodePriority) * pq->capacity);
    }
    NodePriority newNode = {node_id, distance};
    int i = pq->size++;
    while (i && newNode.distance < pq->elements[(i - 1) / 2].distance) {
        pq->elements[i] = pq->elements[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->elements[i] = newNode;
}

NodePriority extractMinPriorityQueue(PriorityQueue *pq) {
    NodePriority minElement = pq->elements[0];
    NodePriority lastElement = pq->elements[--pq->size];
    int i = 0;
    while ((i * 2 + 1) < pq->size) {
        int left = i * 2 + 1, right = i * 2 + 2;
        int smallest = left;
        if (right < pq->size && pq->elements[right].distance < pq->elements[left].distance) {
            smallest = right;
        }
        if (pq->elements[smallest].distance > lastElement.distance) break;
        pq->elements[i] = pq->elements[smallest];
        i = smallest;
    }
    pq->elements[i] = lastElement;
    return minElement;
}

void destroyPriorityQueue(PriorityQueue *pq) {
    free(pq->elements);
    free(pq);
}

void 
ssmap_path_create(const struct ssmap * m, int start_id, int end_id)
{
    /* TODO: task 4 */
    if (start_id == end_id) {
        // Trivial case where start and end nodes are the same
        printf("%d\n", start_id);
        return;
    }

    // Initialize previous nodes and distances
    int *prev = calloc(m->nr_nodes, sizeof(int));
    double *dist = malloc(m->nr_nodes * sizeof(double));
    if (!prev || !dist) {
        printf("error: out of memory.\n");
        free(prev);
        free(dist);
        return;
    }
    for (int i = 0; i < m->nr_nodes; ++i) {
        dist[i] = DBL_MAX;
        prev[i] = -1;
    }
    dist[start_id] = 0.0;

    // Create and populate the priority queue
    PriorityQueue *pq = createPriorityQueue(m->nr_nodes);
    insertPriorityQueue(pq, start_id, 0.0);

    while (pq->size > 0) {
        NodePriority u = extractMinPriorityQueue(pq);
        
        // Early exit if we reach the destination node
        if (u.node_id == end_id) {
            break;
        }

        // Relaxation step for all adjacent nodes
        struct node *node_u = m->nodes[u.node_id];
        for (int i = 0; i < node_u->num_ways; i++) {
            struct way *way = m->ways[node_u->num_ids[i]];
            for (int j = 0; j < way->num_nodes - 1; j++) {
                if (way->node_ids[j] == u.node_id || (!way->oneway && way->node_ids[j + 1] == u.node_id)) {
                    int v = way->node_ids[j] == u.node_id ? way->node_ids[j + 1] : way->node_ids[j];
                    if (v >= 0 && v < m->nr_nodes && m->nodes[v] != NULL) { // Validate the node
                        double alt = dist[u.node_id] + distance_between_nodes(node_u, m->nodes[v]);
                        if (alt < dist[v]) {
                            dist[v] = alt;
                            prev[v] = u.node_id;
                            insertPriorityQueue(pq, v, alt);
                        }
                    }
                }
            }
        }
    }

    // Check if a path exists
    if (prev[end_id] == -1) {
        printf("error: could not find a path from node %d to node %d.\n", start_id, end_id);
    } else {
        // Reconstruct the path from end_id to start_id
        int u = end_id;
        int path[m->nr_nodes], path_size = 0;
        while (u != -1) {
            path[path_size++] = u;
            u = prev[u];
        }

        // Print the path in the correct order
        for (int i = path_size - 1; i >= 0; --i) {
            printf("%d", path[i]);
            if (i > 0) {
                printf(" ");
            }
        }
        printf("\n");
    }

    free(prev);
    free(dist);
    destroyPriorityQueue(pq);
}
