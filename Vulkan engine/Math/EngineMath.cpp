#include "EngineMath.h"

Matrix4x4 EngineMath::Multiply(Matrix4x4 a, Matrix4x4 b)
{
    return a * b;
}

Vector4 EngineMath::TransformVector(Vector4 v, Matrix4x4 m)
{
    return  m * v;
}

Matrix4x4 EngineMath::PerspectiveProjectionMatrix(float FOV, float screenAspect, float screenNear, float screenDepth)
{
    float xScale, yScale, C,  D;
    yScale = 1 / (float)tan(FOV / 2);
    xScale = yScale / screenAspect;


    C = screenDepth / (screenDepth - screenNear);

    D = -screenNear * screenDepth / (screenDepth - screenNear);


    return Matrix4x4(
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, C, 1,
        0, 0, D, 0);
}

Matrix4x4 EngineMath::OrthographicProjectionMatrix(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
    float A, B, C, D, E;
    A = 2 / screenWidth;
    B = 2 / screenHeight;
    C = 1 / (screenDepth - screenNear);
    D = 1;
    E = -screenNear / (screenDepth - screenNear);


    return (Matrix4x4(
        Vector4(A, 0, 0, 0),
        Vector4(0, B, 0, 0),
        Vector4(0, 0, C, 0),
        Vector4(0, 0, E, D))) ;//* Matrix4x4(-1,0,0,0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ))* Matrix4x4(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);


}

Matrix4x4 EngineMath::Identity()
{
    return Matrix4x4(
        Vector4(1, 0, 0, 0),
        Vector4(0, 1, 0, 0),
        Vector4(0, 0, 1, 0),
        Vector4(0, 0, 0, 1));
}

Matrix4x4 EngineMath::LookAtMatrix(Vector3 Eye, Vector3 At, Vector3 up)
{
    Vector3 zaxis, xaxis, yaxis;
    zaxis = Normalized(At - Eye );
    xaxis = Normalized(zaxis.CrossProdut(up,  zaxis));
    yaxis = zaxis.CrossProdut(zaxis, xaxis);
    Matrix4x4 a = 
     Matrix4x4(
        xaxis.x, yaxis.x, zaxis.x, 0,
        xaxis.y, yaxis.y, zaxis.y, 0,
        xaxis.z, yaxis.z, zaxis.z, 0,
        -(xaxis * Eye), -(yaxis * Eye), -(zaxis * Eye), 1);
    return a;
}

Matrix4x4 EngineMath::RotationPitchYawRoll(float P, float Y, float R) // p = x Y = y r = z
{
    float pitch = P * Deg2Rad;
    float yaw = Y * Deg2Rad;
    float roll = R * Deg2Rad;
    Matrix4x4 p, y, r;
    y = Matrix4x4(
       (float)cos(yaw), 0, (float)-sin(yaw), 0,
        0, 1, 0, 0,
        (float)sin(yaw), 0, (float)cos(yaw), 0,
        0, 0, 0, 1);
    r = Matrix4x4(
        (float)cos(roll), (float)sin(roll), 0, 0,
        (float)-sin(roll), (float)cos(roll), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    p = Matrix4x4(
        1, 0, 0, 0,
        0, (float)cos(pitch), (float)sin(pitch),0,
        0, (float)-sin(pitch), (float)cos(pitch), 0,
        0, 0, 0, 1);

    Matrix4x4 buff = (r.operator*(p));
    return buff.operator*(y);
}

Matrix4x4 EngineMath::RotationPitchYawRoll(Vector3 rpy)
{
    float pitch = rpy.x * Deg2Rad;
    float yaw = rpy.y * Deg2Rad;
    float roll = rpy.z * Deg2Rad;
    Matrix4x4 p, y, r;
    y = Matrix4x4(
        (float)cos(yaw), 0, (float)-sin(yaw), 0,
        0, 1, 0, 0,
        (float)sin(yaw), 0, (float)cos(yaw), 0,
        0, 0, 0, 1);
    r = Matrix4x4(
        (float)cos(roll), (float)sin(roll), 0, 0,
        (float)-sin(roll), (float)cos(roll), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    p = Matrix4x4(
        1, 0, 0, 0,
        0, (float)cos(pitch), (float)sin(pitch), 0,
        0, (float)-sin(pitch), (float)cos(pitch), 0,
        0, 0, 0, 1);

    Matrix4x4 buff = (r.operator*(p));
    return buff.operator*(y);
}

//----------------------------------------
Matrix4x4 EngineMath::TransformationMatrix(Vector3 posistion, Vector3 Rotation, Vector3 Scale)
{
    Matrix4x4 scale, translation, rotation;
    scale = ScaleMatrix(Scale);
    translation = TranslationMatrix(posistion);
    rotation = RotationPitchYawRoll(Rotation);
    auto a = rotation * translation;
    return (translation * scale) * rotation;
}
Matrix4x4 EngineMath::TransformationMatrix(Vector3 posistion, Vector3 Rotation, float Scale)
{
    Matrix4x4 scale, translation, rotation;
    scale = ScaleMatrix(Scale);
    translation = TranslationMatrix(posistion);
    rotation = RotationPitchYawRoll(Rotation);
    auto a = rotation * translation;
    return (translation * scale) * rotation;
}
//----------------------------------------
Matrix4x4 EngineMath::TranslationScaleMatrix(Vector3 translation, Vector3 scale)
{
    return TranslationMatrix(translation) * ScaleMatrix(scale);
}
Matrix4x4 EngineMath::TranslationScaleMatrix(Vector3 translation, float scale)
{
    return TranslationMatrix(translation) * ScaleMatrix(scale);

}
//----------------------------------------
Matrix4x4 EngineMath::TranslationMatrix(Vector3 translation)
{
    Matrix4x4 a = Identity();
    a._m03 = translation.x;
    a._m13 = translation.y;
    a._m23 = translation.z;

    return a;
}
//----------------------------------------
Matrix4x4 EngineMath::ScaleMatrix(Vector3 scale)
{
    Matrix4x4 a = Identity();
    a._m00 = scale.x;
    a._m11 = scale.y;
    a._m22 = scale.z;

    return a;
}
Matrix4x4 EngineMath::ScaleMatrix(float scale)
{
    Matrix4x4 a = Identity();
    a._m00 = scale;
    a._m11 = scale;
    a._m22 = scale;
    return a;
}
//----------------------------------------

float EngineMath::PlaneDotCoord(Plane plane, Vector3 point)
{
    return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d * 1;
}
Vector3 EngineMath::Normalized(Vector3 v)
{
    if(v != Vector3(0,0,0))
    return v / v.Length();
    return Vector3(0, 0, 0);
}

Matrix4x4 EngineMath::Transpose(Matrix4x4 m)
{
    return m.Transposed();
}