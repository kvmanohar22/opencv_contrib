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
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
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

#ifndef __NORM2_HPP__
#define __NORM2_HPP__

template <typename _Tp> struct is_norm2_type :
    std::integral_constant<bool, !std::is_unsigned<_Tp>::value && !std::is_same<_Tp, char>::value> {};

template <typename _Tp, int cn> static inline typename std::enable_if< is_norm2_type<_Tp>::value, _Tp >::
    type norm2(cv::Vec<_Tp, cn> a, cv::Vec<_Tp, cn> b) { return (a - b).dot(a - b); }

template <typename _Tp> static inline typename std::enable_if< is_norm2_type<_Tp>::value, _Tp >::
    type norm2(const _Tp &a, const _Tp &b) { return (a - b)*(a - b); }

#endif /* __NORM2_HPP__ */