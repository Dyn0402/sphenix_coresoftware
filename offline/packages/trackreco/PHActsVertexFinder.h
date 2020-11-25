#ifndef TRACKRECO_PHACTSVERTEXFINDER_H
#define TRACKRECO_PHACTSVERTEXFINDER_H

#include "PHInitVertexing.h"
#include "ActsTrackingGeometry.h"

#include <trackbase/TrkrDefs.h>

#include <Acts/Utilities/Result.hpp>
#include <Acts/Vertexing/Vertex.hpp>

#include <ActsExamples/EventData/TrkrClusterMultiTrajectory.hpp>

class PHCompositeNode;
class SvtxTrack;
class SvtxTrackMap;
class SvtxVertexMap;
class SvtxVertex;

namespace Acts
{
  class TrackParameters;
}

using Trajectory = ActsExamples::TrkrClusterMultiTrajectory;
using VertexVector = std::vector<Acts::Vertex<Acts::BoundTrackParameters>>;
using TrackPtrVector = std::vector<const Acts::BoundTrackParameters*>;
using VertexMap = std::map<unsigned int, 
                           Acts::Vertex<Acts::BoundTrackParameters>>;


/**
 * This class calls the Acts::IterativeVertexFinder which takes a 
 * list of tracks and returns a list of vertices that are found.
 */
class PHActsVertexFinder: public PHInitVertexing 
{
  
 public:
  PHActsVertexFinder(const std::string &name = "PHActsVertexFinder");

  virtual ~PHActsVertexFinder() {}

  void setMaxVertices(int maxVertices)
    { m_maxVertices = maxVertices; }

 protected:
  int Setup(PHCompositeNode *topNode) override;
  int Process(PHCompositeNode *topNode) override;
  int ResetEvent(PHCompositeNode *topNode) override;
  int End(PHCompositeNode *topNode) override;

 private:
  
  int createNodes(PHCompositeNode *topNode);
  int getNodes(PHCompositeNode *topNode);

  /// Get list of tracks from PHActsTrkFitter to vertex fit
  std::vector<const Acts::BoundTrackParameters*> getTracks(
             std::map<const double, const unsigned int>& keyMap);

  /// Call acts vertex finder
  VertexVector findVertices(TrackPtrVector& tracks);

  /// Fill maps with relevant vertex information 
  void fillVertexMap(VertexVector& vertices,
		     std::map<const double, const unsigned int>& keyMap);
  
  /// The acts trajectories from PHActsTrkFitter
  std::map<const unsigned int, Trajectory> *m_actsFitResults;

  /// An Acts vertex object map
  VertexMap *m_actsVertexMap;

  int m_event = 0;
  /// Maximum number of vertices that the Acts finder is allowed
  /// to find
  int m_maxVertices = 20;
  SvtxVertexMap *m_svtxVertexMap = nullptr;
  ActsTrackingGeometry *m_tGeometry = nullptr;

  /// Boolean to place m_actsVertexMap on node tree
  /// False by default since Svtx objects are still the default
  bool m_addActsVertexNode = false;
    
};

#endif // TRACKRECO_PHACTSVERTEXFINDER_H