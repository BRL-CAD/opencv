// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ifndef OPENCV_3D_ODOMETRY_HPP
#define OPENCV_3D_ODOMETRY_HPP

#include <opencv2/core.hpp>

#include "odometry_frame.hpp"
#include "odometry_settings.hpp"

namespace cv
{

/** These constants are used to set a type of data which odometry will use
* @param DEPTH     only depth data
* @param RGB       only rgb image
* @param RGB_DEPTH depth and rgb data simultaneously
*/
enum OdometryType
{
    DEPTH     = 0,
    RGB       = 1,
    RGB_DEPTH = 2
};

/** These constants are used to set the speed and accuracy of odometry
* @param COMMON accurate but not so fast
* @param FAST   less accurate but faster
*/
enum class OdometryAlgoType
{
    COMMON = 0,
    FAST = 1
};

class CV_EXPORTS_W Odometry
{
public:
    CV_WRAP Odometry();
    CV_WRAP Odometry(OdometryType otype);
    Odometry(OdometryType otype, const OdometrySettings settings, OdometryAlgoType algtype);
    ~Odometry();

    /** Prepare frame for odometry calculation
     * @param frame odometry prepare this frame as src frame and dst frame simultaneously
     */
    void prepareFrame(OdometryFrame& frame) const;

    /** Prepare frame for odometry calculation
     * @param srcFrame frame will be prepared as src frame ("original" image)
     * @param dstFrame frame will be prepared as dsr frame ("rotated" image)
     */
    void prepareFrames(OdometryFrame& srcFrame, OdometryFrame& dstFrame) const;

    /** Compute Rigid Transformation between two frames so that Rt * src = dst
     * @param srcFrame src frame ("original" image)
     * @param dstFrame dst frame ("rotated" image)
     * @param Rt Rigid transformation, which will be calculated, in form:
     * { R_11 R_12 R_13 t_1
     *   R_21 R_22 R_23 t_2
     *   R_31 R_32 R_33 t_3
     *   0    0    0    1  }
     */
    bool compute(const OdometryFrame& srcFrame, const OdometryFrame& dstFrame, OutputArray Rt) const;

    CV_WRAP bool compute(InputArray srcDepthFrame, InputArray dstDepthFrame, OutputArray Rt) const;
    CV_WRAP bool compute(InputArray srcDepthFrame, InputArray srcRGBFrame, InputArray dstDepthFrame, InputArray dstRGBFrame, OutputArray Rt) const;

    /**
     * @brief Get the normals computer object used for normals calculation (if presented).
     * The normals computer is generated at first need during prepareFrame when normals are required for the ICP algorithm
     * but not presented by a user. Re-generated each time the related settings change or a new frame arrives with the different size.
     */
    Ptr<RgbdNormals> getNormalsComputer() const;

    class Impl;
private:
    Ptr<Impl> impl;
};

}
#endif //OPENCV_3D_ODOMETRY_HPP
