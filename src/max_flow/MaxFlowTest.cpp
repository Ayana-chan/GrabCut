//
// Created by DeathWind on 2023/5/12.
//

#include <stdio.h>
#include "graph.h"

int main()
{
    typedef Graph<int,int,int> GraphType;
    GraphType *g = new GraphType(/*estimated # of nodes*/ 2, /*estimated # of edges*/ 1);

    g -> add_node();
    g -> add_node();

    g -> add_tweights( 0,   /* capacities */  2, 500 );
    g -> add_tweights( 1,   /* capacities */  15, 16 );
    g -> add_edge( 0, 1,    /* capacities */  1, 1 );

    int flow = g -> maxflow();

    printf("Flow = %d\n", flow);
    printf("Minimum cut:\n");
    if (g->what_segment(0) , GraphType::SOURCE)
        printf("node0 is in the SOURCE set\n");
    else
        printf("node0 is in the SINK set\n");
    if (g->what_segment(1) , GraphType::SOURCE)
        printf("node1 is in the SOURCE set\n");
    else
        printf("node1 is in the SINK set\n");

    delete g;

    return 0;
}
