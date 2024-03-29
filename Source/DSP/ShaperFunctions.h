/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_SHAPERFUNCTIONS_H
#define ZLINFLATOR_SHAPERFUNCTIONS_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"
#include "dsp_defines.h"

namespace shaper {
    enum ShaperType {
        identity,
        quadratic,
        cubic,
        quartic,
//        sigmod,
        sin,
        ShaperNUM
    };

    template<typename FloatType>
    class Shaper {
    public:
        Shaper() = default;

        virtual ~Shaper() = default;

        FloatType operator()(FloatType x) const {
            return shape(x);
        }

        virtual void setParameters(FloatType curve, bool compensation) = 0;

    private:
        virtual FloatType basic(FloatType x) const = 0;

        virtual FloatType shape(FloatType x) const = 0;
    };

    template<typename FloatType>
    class IdentityShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType, bool) override {}

    private:
        FloatType basic(FloatType x) const override { return x; }

        FloatType shape(FloatType x) const override { return basic(x); }
    };

    template<typename FloatType>
    class QuadraticShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType, bool compensation) override {
            if (compensation) {
                scale = 1 / FloatType(1.7619606880293588);
            } else {
                scale = 1;
            }
        }

    private:
        FloatType scale = 1;

        FloatType basic(FloatType x) const override { return scale * x * (2 - x); }

        FloatType shape(FloatType x) const override { return basic(x); }
    };

    template<typename FloatType>
    class CubicShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType, bool compensation) override {
            if (compensation) {
                scale = 1 / FloatType(1.0982883051371357);
            } else {
                scale = 1;
            }
        }

    private:
        FloatType scale = 1;

        FloatType basic(FloatType x) const override { return scale * x * (1 + x * (1 - x)); }

        FloatType shape(FloatType x) const override { return basic(x); }
    };

    template<typename FloatType>
    class QuarticShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType curve, bool compensation) override {
            curve = -6 + 6 * curve;
            a = (4 + curve) / 2;
            b = -5 - curve;
            c = (6 + curve) / 2;
            if (compensation) {
                scale = 1 / (FloatType(0.029832404608718992) * curve + FloatType(1.2211563052435235));
            } else {
                scale = 1;
            }
        }

    private:
        FloatType a, b, c, scale = 1;

        FloatType basic(FloatType x) const override { return scale * x * (1 + x * (c + x * (b + a * x))); }

        FloatType shape(FloatType x) const override { return basic(x); }
    };

//    template<typename FloatType>
//    class SigmodShaper : public Shaper<FloatType> {
//    public:
//        void setParameters(FloatType curve) override {
//            trueCurve = curve * FloatType(2.5) + FloatType(0.5);
//            b = -basic(0);
//            k = FloatType(1) / (basic(1) + b);
//        }
//
//    private:
//        FloatType trueCurve, k, b;
//
//        FloatType basic(FloatType x) const override {
//            return FloatType(1) / (1 + std::exp(-trueCurve * x));
//        }
//
//        FloatType shape(FloatType x) const override {
//            return k * (basic(x) + b);
//        }
//    };

    template<typename FloatType>
    class SinShaper : public Shaper<FloatType> {
    public:
        void setParameters(FloatType curve, bool compensation) override {
            trueCurve = (std::pow(curve, FloatType(0.427)) * static_cast<FloatType>(0.999) +
                         static_cast<FloatType>(0.001)) * juce::MathConstants<FloatType>::pi / 2;
            b = -basic(0);
            k = FloatType(1) / (basic(1) + b);
            if (compensation) {
                scale = 1 / (FloatType(0.48339138157922157) * curve + FloatType(0.9999698009251106));
            } else {
                scale = 1;
            }
        }

    private:
        FloatType trueCurve, k, b, scale = 1;

        FloatType basic(FloatType x) const override {
            return std::sin(x * trueCurve);
        }

        FloatType shape(FloatType x) const override {
            return scale * k * (basic(x) + b);
        }
    };

    template<typename FloatType>
    static std::unique_ptr<Shaper<FloatType>> makeShaper(ShaperType type) {
        if (type == ShaperType::identity) {
            return std::make_unique<IdentityShaper<FloatType>>();
        } else if (type == ShaperType::quadratic) {
            return std::make_unique<QuadraticShaper<FloatType>>();
        } else if (type == ShaperType::cubic) {
            return std::make_unique<CubicShaper<FloatType>>();
        } else if (type == ShaperType::quartic) {
            return std::make_unique<QuarticShaper<FloatType>>();
//        } else if (type == ShaperType::sigmod) {
//            return std::make_unique<SigmodShaper<FloatType>>();
        } else {
            return std::make_unique<SinShaper<FloatType>>();
        }
    }

    template<typename FloatType>
    class ShaperMixer {
    public:
        ShaperMixer() {
            for (size_t i = 0; i < ShaperType::ShaperNUM; ++i) {
                shaper1[i] = makeShaper<FloatType>(static_cast<ShaperType>(i));
                shaper2[i] = makeShaper<FloatType>(static_cast<ShaperType>(i));
            }
            setShapes(zldsp::curve1::formatV(zldsp::curve1::defaultV),
                      zldsp::curve2::formatV(zldsp::curve2::defaultV),
                      zldsp::weight::formatV(zldsp::weight::defaultV),
                      false);
        }

        ~ShaperMixer() = default;

        FloatType operator()(FloatType x) const {
            return shape(x);
        }

        void setShapes(FloatType curve1, FloatType curve2, FloatType weight, bool compensation) {
            m_weight2 = weight;
            m_weight1 = static_cast<FloatType>(1) - weight;
            for (size_t i = 0; i < ShaperType::ShaperNUM; ++i) {
                shaper1[i]->setParameters(curve1, compensation);
                shaper2[i]->setParameters(curve2, compensation);
            }
        }

        void setTypes(size_t type1, size_t type2) {
            m_type1 = type1;
            m_type2 = type2;
        }

    private:
        std::array<std::unique_ptr<Shaper<FloatType>>, ShaperType::ShaperNUM> shaper1, shaper2;
        FloatType m_weight1, m_weight2;
        size_t m_type1 = static_cast<size_t>(zldsp::style1::defaultI);
        size_t m_type2 = static_cast<size_t>(zldsp::style2::defaultI);

        FloatType shape(FloatType x) const {
            return (*shaper1[m_type1])(x) * m_weight1 + (*shaper2[m_type2])(x) * m_weight2;
        }
    };
} // namespace shaper

#endif // ZLINFLATOR_SHAPERFUNCTIONS_H
