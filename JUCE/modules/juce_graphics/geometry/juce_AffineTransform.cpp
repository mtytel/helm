/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

AffineTransform::AffineTransform (float m00, float m01, float m02,
                                  float m10, float m11, float m12) noexcept
 :  mat00 (m00), mat01 (m01), mat02 (m02),
    mat10 (m10), mat11 (m11), mat12 (m12)
{
}

bool AffineTransform::operator== (const AffineTransform& other) const noexcept
{
    return mat00 == other.mat00
        && mat01 == other.mat01
        && mat02 == other.mat02
        && mat10 == other.mat10
        && mat11 == other.mat11
        && mat12 == other.mat12;
}

bool AffineTransform::operator!= (const AffineTransform& other) const noexcept
{
    return ! operator== (other);
}

//==============================================================================
bool AffineTransform::isIdentity() const noexcept
{
    return mat01 == 0.0f
        && mat02 == 0.0f
        && mat10 == 0.0f
        && mat12 == 0.0f
        && mat00 == 1.0f
        && mat11 == 1.0f;
}

JUCE_DECLARE_DEPRECATED_STATIC (const AffineTransform AffineTransform::identity (1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);)

//==============================================================================
AffineTransform AffineTransform::followedBy (const AffineTransform& other) const noexcept
{
    return { other.mat00 * mat00 + other.mat01 * mat10,
             other.mat00 * mat01 + other.mat01 * mat11,
             other.mat00 * mat02 + other.mat01 * mat12 + other.mat02,
             other.mat10 * mat00 + other.mat11 * mat10,
             other.mat10 * mat01 + other.mat11 * mat11,
             other.mat10 * mat02 + other.mat11 * mat12 + other.mat12 };
}

AffineTransform AffineTransform::translated (float dx, float dy) const noexcept
{
    return { mat00, mat01, mat02 + dx,
             mat10, mat11, mat12 + dy };
}

AffineTransform AffineTransform::translation (float dx, float dy) noexcept
{
    return { 1.0f, 0.0f, dx,
             0.0f, 1.0f, dy };
}

AffineTransform AffineTransform::withAbsoluteTranslation (float tx, float ty) const noexcept
{
    return { mat00, mat01, tx,
             mat10, mat11, ty };
}

AffineTransform AffineTransform::rotated (float rad) const noexcept
{
    auto cosRad = std::cos (rad);
    auto sinRad = std::sin (rad);

    return { cosRad * mat00 - sinRad * mat10,
             cosRad * mat01 - sinRad * mat11,
             cosRad * mat02 - sinRad * mat12,
             sinRad * mat00 + cosRad * mat10,
             sinRad * mat01 + cosRad * mat11,
             sinRad * mat02 + cosRad * mat12 };
}

AffineTransform AffineTransform::rotation (float rad) noexcept
{
    auto cosRad = std::cos (rad);
    auto sinRad = std::sin (rad);

    return { cosRad, -sinRad, 0,
             sinRad,  cosRad, 0 };
}

AffineTransform AffineTransform::rotation (float rad, float pivotX, float pivotY) noexcept
{
    auto cosRad = std::cos (rad);
    auto sinRad = std::sin (rad);

    return { cosRad, -sinRad, -cosRad * pivotX +  sinRad * pivotY + pivotX,
             sinRad,  cosRad, -sinRad * pivotX + -cosRad * pivotY + pivotY };
}

AffineTransform AffineTransform::rotated (float angle, float pivotX, float pivotY) const noexcept
{
    return followedBy (rotation (angle, pivotX, pivotY));
}

AffineTransform AffineTransform::scaled (float factorX, float factorY) const noexcept
{
    return { factorX * mat00, factorX * mat01, factorX * mat02,
             factorY * mat10, factorY * mat11, factorY * mat12 };
}

AffineTransform AffineTransform::scaled (float factor) const noexcept
{
    return { factor * mat00, factor * mat01, factor * mat02,
             factor * mat10, factor * mat11, factor * mat12 };
}

AffineTransform AffineTransform::scale (float factorX, float factorY) noexcept
{
    return { factorX, 0, 0, 0, factorY, 0 };
}

AffineTransform AffineTransform::scale (float factor) noexcept
{
    return { factor, 0, 0, 0, factor, 0 };
}

AffineTransform AffineTransform::scaled (float factorX, float factorY,
                                         float pivotX, float pivotY) const noexcept
{
    return { factorX * mat00, factorX * mat01, factorX * mat02 + pivotX * (1.0f - factorX),
             factorY * mat10, factorY * mat11, factorY * mat12 + pivotY * (1.0f - factorY) };
}

AffineTransform AffineTransform::scale (float factorX, float factorY,
                                        float pivotX, float pivotY) noexcept
{
    return { factorX, 0, pivotX * (1.0f - factorX),
             0, factorY, pivotY * (1.0f - factorY) };
}

AffineTransform AffineTransform::shear (float shearX, float shearY) noexcept
{
    return { 1.0f,   shearX, 0,
             shearY, 1.0f,   0 };
}

AffineTransform AffineTransform::sheared (float shearX, float shearY) const noexcept
{
    return { mat00 + shearX * mat10,
             mat01 + shearX * mat11,
             mat02 + shearX * mat12,
             mat10 + shearY * mat00,
             mat11 + shearY * mat01,
             mat12 + shearY * mat02 };
}

AffineTransform AffineTransform::verticalFlip (float height) noexcept
{
    return { 1.0f,  0.0f, 0.0f,
             0.0f, -1.0f, height };
}

AffineTransform AffineTransform::inverted() const noexcept
{
    double determinant = (mat00 * mat11 - mat10 * mat01);

    if (determinant != 0)
    {
        determinant = 1.0 / determinant;

        auto dst00 = (float) ( mat11 * determinant);
        auto dst10 = (float) (-mat10 * determinant);
        auto dst01 = (float) (-mat01 * determinant);
        auto dst11 = (float) ( mat00 * determinant);

        return { dst00, dst01, -mat02 * dst00 - mat12 * dst01,
                 dst10, dst11, -mat02 * dst10 - mat12 * dst11 };
    }

    // singularity..
    return *this;
}

bool AffineTransform::isSingularity() const noexcept
{
    return (mat00 * mat11 - mat10 * mat01) == 0.0f;
}

AffineTransform AffineTransform::fromTargetPoints (float x00, float y00,
                                                   float x10, float y10,
                                                   float x01, float y01) noexcept
{
    return { x10 - x00, x01 - x00, x00,
             y10 - y00, y01 - y00, y00 };
}

AffineTransform AffineTransform::fromTargetPoints (float sx1, float sy1, float tx1, float ty1,
                                                   float sx2, float sy2, float tx2, float ty2,
                                                   float sx3, float sy3, float tx3, float ty3) noexcept
{
    return fromTargetPoints (sx1, sy1, sx2, sy2, sx3, sy3)
            .inverted()
            .followedBy (fromTargetPoints (tx1, ty1, tx2, ty2, tx3, ty3));
}

bool AffineTransform::isOnlyTranslation() const noexcept
{
    return mat01 == 0.0f
        && mat10 == 0.0f
        && mat00 == 1.0f
        && mat11 == 1.0f;
}

float AffineTransform::getScaleFactor() const noexcept
{
    return (std::abs (mat00) + std::abs (mat11)) / 2.0f;
}

} // namespace juce
