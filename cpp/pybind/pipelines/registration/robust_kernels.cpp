
// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------
// @author Ignacio Vizzo     [ivizzo@uni-bonn.de]
//
// Copyright (c) 2020 Ignacio Vizzo, Cyrill Stachniss, University of Bonn.
// ----------------------------------------------------------------------------

#include <pybind11/attr.h>
#include <pybind11/pybind11.h>

#include <memory>

#include "open3d/pipelines/registration/RobustKernel.h"
#include "open3d/utility/Console.h"
#include "pybind/docstring.h"
#include "pybind/pipelines/registration/registration.h"

namespace open3d {
namespace pipelines {
namespace registration {

template <class RobustKernelBase = RobustKernel>
class PyRobustKernelT : public RobustKernelBase {
public:
    using RobustKernelBase::RobustKernelBase;
    double Weight(double residual) const override {
        PYBIND11_OVERLOAD_PURE(double, RobustKernelBase, residual);
    }
};

// Type aliases to improve readability
using PyRobustKernel = PyRobustKernelT<RobustKernel>;
using PyL2Loss = PyRobustKernelT<L2Loss>;
using PyL1Loss = PyRobustKernelT<L1Loss>;
using PyHuberLoss = PyRobustKernelT<HuberLoss>;
using PyCauchyLoss = PyRobustKernelT<CauchyLoss>;
using PyGMLoss = PyRobustKernelT<GMLoss>;
using PyTukeyLoss = PyRobustKernelT<TukeyLoss>;

void pybind_robust_kernels(py::module &m) {
    // open3d.registration.RobustKernel
    py::class_<RobustKernel, std::shared_ptr<RobustKernel>, PyRobustKernel> rk(
            m, "RobustKernel");
    rk.def("weight", &RobustKernel::Weight, "residual"_a,
           "Obtain the weight for the given residual according to the "
           "robust kernel model.");
    docstring::ClassMethodDocInject(
            m, "RobustKernel", "weight",
            {{"residual", "value obtained during the optimization problem"}});

    // open3d.registration.L2Loss
    py::class_<L2Loss, std::shared_ptr<L2Loss>, PyL2Loss, RobustKernel> l2_loss(
            m, "L2Loss");
    py::detail::bind_default_constructor<L2Loss>(l2_loss);
    py::detail::bind_copy_functions<L2Loss>(l2_loss);
    l2_loss.def("__repr__", [](const L2Loss &rk) {
        (void)rk;
        return "RobustKernel::L2Loss";
    });

    // open3d.registration.L1Loss:RobustKernel
    py::class_<L1Loss, std::shared_ptr<L1Loss>, PyL1Loss, RobustKernel> l1_loss(
            m, "L1Loss");
    py::detail::bind_default_constructor<L1Loss>(l1_loss);
    py::detail::bind_copy_functions<L1Loss>(l1_loss);
    l1_loss.def("__repr__", [](const L1Loss &rk) {
        (void)rk;
        return "RobustKernel::L1Loss";
    });

    // open3d.registration.HuberLoss
    py::class_<HuberLoss, std::shared_ptr<HuberLoss>, PyHuberLoss, RobustKernel>
            h_loss(m, "HuberLoss");
    py::detail::bind_copy_functions<HuberLoss>(h_loss);
    h_loss.def(py::init(
                       [](double k) { return std::make_shared<HuberLoss>(k); }),
               "k"_a)
            .def("__repr__",
                 [](const HuberLoss &rk) {
                     return std::string("RobustKernel::HuberLoss with k=") +
                            std::to_string(rk.k_);
                 })
            .def_readwrite("k", &HuberLoss::k_, "Parameter of the loss");

    // open3d.registration.CauchyLoss
    py::class_<CauchyLoss, std::shared_ptr<CauchyLoss>, PyCauchyLoss,
               RobustKernel>
            c_loss(m, "CauchyLoss");
    py::detail::bind_copy_functions<CauchyLoss>(c_loss);
    c_loss.def(py::init([](double k) {
                   return std::make_shared<CauchyLoss>(k);
               }),
               "k"_a)
            .def("__repr__",
                 [](const CauchyLoss &rk) {
                     return std::string("RobustKernel::CauchyLoss with k=") +
                            std::to_string(rk.k_);
                 })
            .def_readwrite("k", &CauchyLoss::k_, "Parameter of the loss.");

    // open3d.registration.GMLoss
    py::class_<GMLoss, std::shared_ptr<GMLoss>, PyGMLoss, RobustKernel> gm_loss(
            m, "GMLoss");
    py::detail::bind_copy_functions<GMLoss>(gm_loss);
    gm_loss.def(py::init([](double k) { return std::make_shared<GMLoss>(k); }),
                "k"_a)
            .def("__repr__",
                 [](const GMLoss &rk) {
                     return std::string("RobustKernel::GMLoss with k=") +
                            std::to_string(rk.k_);
                 })
            .def_readwrite("k", &GMLoss::k_, "Parameter of the loss.");

    // open3d.registration.TukeyLoss:RobustKernel
    py::class_<TukeyLoss, std::shared_ptr<TukeyLoss>, PyTukeyLoss, RobustKernel>
            t_loss(m, "TukeyLoss");
    py::detail::bind_copy_functions<TukeyLoss>(t_loss);
    t_loss.def(py::init(
                       [](double k) { return std::make_shared<TukeyLoss>(k); }),
               "k"_a)
            .def("__repr__",
                 [](const TukeyLoss &tk) {
                     return std::string("RobustKernel::TukeyLoss with k=") +
                            std::to_string(tk.k_);
                 })
            .def_readwrite("k", &TukeyLoss::k_,
                           "``k`` Is a tunning constant for the loss.");
}  // namespace pipelines

}  // namespace registration
}  // namespace pipelines
}  // namespace open3d
