/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef _OPENCV_DNN_OBJDETECT_CORE_DETECT_HPP_
#define _OPENCV_DNN_OBJDETECT_CORE_DETECT_HPP_

#include <vector>
#include <memory>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace cv
{
namespace dnn_objdetect
{

    typedef struct
    {
      int xmin, xmax;
      int ymin, ymax;
      int class_idx;
      std::string label_name;
      double class_prob;
    } object;

    /** @brief A class to post process model predictions
     */
    class CV_EXPORTS InferBbox
    {
      public:
        /** @brief Default constructer
        @param _delta_bbox Blob containing relative coordinates of bounding boxes
        @param _class_scores Blob containing the probability values of each class
        @param _conf_scores Blob containing the confidence scores
         */
        InferBbox(Mat _delta_bbox, Mat _class_scores, Mat _conf_scores);

        /** @brief Filters the bounding boxes.
         */
        void filter(double thresh =  0.8);

        /** @brief Vector which holds the final detections of the model
         */
        std::vector<object> detections;

      protected:
        /** @brief Transform relative coordinates from ConvDet to bounding box coordinates
        @param bboxes Vector to hold the predicted bounding boxes
         */
        void transform_bboxes(std::vector<std::vector<double> > *bboxes);

        /** @brief Computes final probability values of each bounding box
        @param final_probs Vector to hold the probability values
         */
        void final_probability_dist(std::vector<std::vector<double> > *final_probs);

        /** @brief Transform bounding boxes from [x, y, h, w] to [xmin, ymin, xmax, ymax]
        @param pre Vector conatining initial co-ordinates
        @param post Vector containing the transformed co-ordinates
         */
        void transform_bboxes_inv(std::vector<std::vector<double> > *pre,
                                  std::vector<std::vector<double> > *post);

        /** @brief Ensures that the bounding box values are within image boundaries
        @param min_max_boxes Vector containing bounding boxes of the form [xmin, ymin, xmax, ymax]
         */
        void assert_predictions(std::vector<std::vector<double> > *min_max_boxes);

        /** @brief Filter top `n` predictions
        @param probs Final probability values of bounding boxes
        @param boxes Predicted bounding box co-ordinates
        @param top_n_boxes Contains bounding box co-ordinates of top `n` boxes
        @param top_n_idxs Containes class indices of top `n` bounding boxes
        @param top_n_probs Contains probability values of top `n` bounding boxes
         */
        void filter_top_n(std::vector<std::vector<double> > *probs,
                          std::vector<std::vector<double> > *boxes,
                          std::vector<std::vector<double> > &top_n_boxes,
                          std::vector<size_t> &top_n_idxs,
                          std::vector<double> &top_n_probs);

        /** @brief Wrapper to apply Non-Maximal Supression
        @param top_n_boxes Contains bounding box co-ordinates of top `n` boxes
        @param top_n_idxs Containes class indices of top `n` bounding boxes
        @param top_n_probs Contains probability values of top `n` bounding boxes
         */
        void nms_wrapper(std::vector<std::vector<double> > &top_n_boxes,
                         std::vector<size_t> &top_n_idxs,
                         std::vector<double> &top_n_probs);

       /** @brief Applies Non-Maximal Supression
       @param boxes Bounding box co-ordinates belonging to one class
       @param probs Probability values of boxes belonging to one class
        */
        std::vector<bool> non_maximal_suppression(std::vector<std::vector<double> >
                                         *boxes, std::vector<double> *probs);

       /** @brief Computes intersection over union of bounding boxes
       @param boxes Vector of bounding box co-ordinates
       @param base_box Base box wrt which IOU is calculated
       @param iou Vector to store IOU values
        */
        void intersection_over_union(std::vector<std::vector<double> > *boxes,
                          std::vector<double> *base_box, std::vector<float> *iou);

        static inline bool comparator (std::pair<double, size_t> l1,
            std::pair<double, size_t> l2)
        {
          return l1.first > l2.first;
        }

      private:
        Mat delta_bbox;
        Mat class_scores;
        Mat conf_scores;

        size_t image_width;
        size_t image_height;

        size_t W, H;
        std::vector<std::vector<double> > anchors_values;
        std::vector<std::pair<double, double> > anchor_center;
        std::vector<std::pair<double, double> > anchor_shapes;

        std::vector<std::string> label_map;

        size_t num_classes;
        size_t anchors_per_grid;
        size_t anchors;
        double intersection_thresh;
        double nms_intersection_thresh;
        size_t n_top_detections;
        double epsilon;
    };

}  //  namespace dnn_objdetect

}  //  namespace cv

#endif
