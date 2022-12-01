/**
 * @file
 * @brief Definition of module TrackingInfo
 *
 * @copyright Copyright (c) 2020 CERN and the Corryvreckan authors.
 * This software is distributed under the terms of the MIT License, copied verbatim in the file "LICENSE.md".
 * In applying this license, CERN does not waive the privileges and immunities granted to it by virtue of its status as an
 * Intergovernmental Organization or submit itself to any jurisdiction.
 */

#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <iostream>
#include "core/module/Module.hpp"
#include "objects/Cluster.hpp"
#include "objects/Pixel.hpp"
#include "objects/Track.hpp"

namespace corryvreckan {
    /** @ingroup Modules
     * @brief Module to do function
     *
     * More detailed explanation of module
     */
    class TrackingInfo : public Module {

    public:
        /**
         * @brief Constructor for this unique module
         * @param config Configuration object for this module as retrieved from the steering file
         * @param detectors Vector of pointers to the detectors
         */
        TrackingInfo(Configuration& config, std::vector<std::shared_ptr<Detector>> detectors);

        /**
         * @brief [Initialise this module]
         */
        void initialize() override;

        /**
         * @brief [Run the function of this module]
         */
        StatusCode run(const std::shared_ptr<Clipboard>& clipboard) override;

        /**
         * @brief [Finalise module]
         */
        void finalize(const std::shared_ptr<ReadonlyClipboard>& clipboard) override;

    private:
        int m_eventNumber;
        double z_dut_;
        double z_ref0_;
        double z_ref1_;
        double z_ref2_;
        double z_ref3_;
        double z_ref4_;
        double z_ref5_;
        TTree* Tracks;
        std::vector<double> x_tel0,y_tel0, x_tel1,y_tel1, x_tel2,y_tel2, x_tel3,y_tel3, x_tel4,y_tel4, x_tel5,y_tel5, x_dut,y_dut = {};
        std::vector<double> x_res0,y_res0, x_res1,y_res1, x_res2,y_res2, x_res3,y_res3, x_res4,y_res4, x_res5,y_res5 = {};
        std::vector<uint32_t> triggerId = {};
        std::vector<double> triggerTimestamp = {};

    };

} // namespace corryvreckan
