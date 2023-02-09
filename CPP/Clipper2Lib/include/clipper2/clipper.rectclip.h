/*******************************************************************************
* Author    :  Angus Johnson                                                   *
* Date      :  9 February 2023                                                 *
* Website   :  http://www.angusj.com                                           *
* Copyright :  Angus Johnson 2010-2023                                         *
* Purpose   :  FAST rectangular clipping                                       *
* License   :  http://www.boost.org/LICENSE_1_0.txt                            *
*******************************************************************************/

#ifndef CLIPPER_RECTCLIP_H
#define CLIPPER_RECTCLIP_H

#include <cstdlib>
#include <vector>
#include <queue>
#include "clipper.h"
#include "clipper.core.h"

namespace Clipper2Lib
{

  enum class Location { Left, Top, Right, Bottom, Inside };

  class OutPt2;
  typedef std::vector<OutPt2*> OutPt2List;

  class OutPt2 {
  public:
    Point64 pt;
    size_t owner_idx;
    OutPt2List* edge;
    OutPt2* next;
    OutPt2* prev;
  };

  //------------------------------------------------------------------------------
  // RectClip
  //------------------------------------------------------------------------------

  class RectClip {
  private:
    void ExecuteInternal(const Path64& path);
    Path64 GetPath(size_t idx);
  protected:
    const Rect64 rect_;
    const Path64 rectPath_;
    const Point64 mp_;
    Rect64 pathRect_;
    std::deque<OutPt2> op_container_;
    OutPt2List results_;  // each path can be broken into multiples
    OutPt2List edges_[8]; // clockwise and counter-clockwise
    std::vector<Location> start_locs_;
    void CheckEdges();
    void TidyEdges(int idx, OutPt2List& cw, OutPt2List& ccw);
    void GetNextLocation(const Path64& path,
      Location& loc, int& i, int highI);
    OutPt2* Add(Point64 pt, bool start_new = false);
    void AddCorner(Location prev, Location curr);
    void AddCorner(Location& loc, bool isClockwise);
  public:
    explicit RectClip(const Rect64& rect) :
      rect_(rect),
      rectPath_(rect.AsPath()),
    mp_(rect.MidPoint()) {}
    Paths64 Execute(const Paths64& paths, bool convex_only = false);
  };

  //------------------------------------------------------------------------------
  // RectClipLines
  //------------------------------------------------------------------------------

  class RectClipLines : public RectClip {
  private:
    void ExecuteInternal(const Path64& path);
    virtual Path64 GetPath(size_t idx);
  public:
    explicit RectClipLines(const Rect64& rect) : RectClip(rect) {};
    Paths64 Execute(const Paths64& paths);
  };

} // Clipper2Lib namespace
#endif  // CLIPPER_RECTCLIP_H
