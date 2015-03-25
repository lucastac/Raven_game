#ifndef ASTAR_HEURISTIC_POLICIES_H
#define ASTAR_HEURISTIC_POLICIES_H
//-----------------------------------------------------------------------------
//
//  Name:   AStarHeuristicPolicies.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class templates defining a heuristic policy for use with the A*
//          search algorithm
//-----------------------------------------------------------------------------
#include "misc/utils.h"

//-----------------------------------------------------------------------------
//the euclidian heuristic (straight-line distance)
//-----------------------------------------------------------------------------
class Heuristic_Euclid 
{
public:

  Heuristic_Euclid(){}

  //calculate the straight line distance from node nd1 to node nd2
  template <class graph_type>
  static double Calculate(const graph_type& G, int nd1, int nd2)
  {
    return Vec2DDistance(G.GetNode(nd1).Pos(), G.GetNode(nd2).Pos());
  }
};

//-----------------------------------------------------------------------------
//this uses the euclidian distance but adds in an amount of noise to the 
//result. You can use this heuristic to provide imperfect paths. This can
//be handy if you find that you frequently have lots of agents all following
//each other in single file to get from one place to another
//-----------------------------------------------------------------------------
class Heuristic_Noisy_Euclidian
{
public:

  Heuristic_Noisy_Euclidian(){}

  //calculate the straight line distance from node nd1 to node nd2
  template <class graph_type>
  static double Calculate(const graph_type& G, int nd1, int nd2)
  {
    return Vec2DDistance(G.GetNode(nd1).Pos(), G.GetNode(nd2).Pos()) * RandInRange(0.9f, 1.1f);
  }
};

//-----------------------------------------------------------------------------
//utiliza os bot na heuristica
//-----------------------------------------------------------------------------
class Heuristic_Bot 
{
public:

  Heuristic_Bot(){}

  
  template <class graph_type>
  static double Calculate(const graph_type& G, int nd1, int nd2, std::list<Vector2D> botPos)
  {
	  double dist = 1000000*botPos.size()*botPos.size() ;
	  double botsValue = 0;

	  std::list<Vector2D>::const_iterator curBot = botPos.begin();
	  for(curBot ; curBot != botPos.end();++curBot)
	  {
		  botsValue += Vec2DDistance(G.GetNode(nd2).Pos(),(*curBot));
	  }

	  //debug_con << dist/botsValue << endl;
	  return dist/(botsValue * botsValue);

  }
};


//-----------------------------------------------------------------------------
//you can use this class to turn the A* algorithm into Dijkstra's search.
//this is because Dijkstra's is equivalent to an A* search using a heuristic
//value that is always equal to zero.
//-----------------------------------------------------------------------------
class Heuristic_Dijkstra 
{
public:

  template <class graph_type>
  static double Calculate(const graph_type& G, int nd1, int nd2)
  {
    return 0;
  }
};






#endif
