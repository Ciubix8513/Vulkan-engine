#include "Matrix4x4.h"


Matrix4x4 EngineMath::Matrix4x4::Identity()
{
    return Matrix4x4(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1);
}

EngineMath::Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    _m00 = m00;
    _m01 = m01;
    _m02 = m02;
    _m03 = m03;

    _m10 = m10;
    _m11 = m11;
    _m12 = m12;
    _m13 = m13;

    _m20 = m20;
    _m21 = m21;
    _m22 = m22;
    _m23 = m23;

    _m30 = m30;
    _m31 = m31;
    _m32 = m32;
    _m33 = m33;
}
EngineMath::Matrix4x4::Matrix4x4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
{
    SetRow(0, row0);
    SetRow(1, row1);
    SetRow(2, row2);
    SetRow(3, row3);
}
EngineMath::Matrix4x4::Matrix4x4()
{
    
    SetRow(0, Vector4(1, 0, 0, 0));
    SetRow(1, Vector4(0, 1, 0, 0));
    SetRow(2, Vector4(0, 0, 1, 0));
    SetRow(3, Vector4(0, 0, 0, 1));

}

Matrix4x4 EngineMath::Matrix4x4::Transposed()
{

    Matrix4x4 m;
    for (int i = 0; i < 4; i++)
        m.SetRow(i, GetRow(i));
    Matrix4x4 Out;

    Out._m00 = m._m00;
    Out._m01 = m._m10;
    Out._m02 = m._m20;
    Out._m03 = m._m30;

    Out._m11 = m._m11;
    Out._m10 = m._m01;
    Out._m12 = m._m21;
    Out._m13 = m._m31;

    Out._m22 = m._m22;
    Out._m21 = m._m12;
    Out._m20 = m._m02;
    Out._m23 = m._m32;

    Out._m30 = m._m03;
    Out._m31 = m._m13;
    Out._m32 = m._m23;
    Out._m33 = m._m33;

    return Out;
}
void EngineMath::Matrix4x4::Transpose()
{
    Matrix4x4 m ;
    for (int i = 0; i < 4; i++)    
        m.SetRow(i,GetRow(i));
    
    _m00 = m._m00;
    _m01 = m._m10;
    _m02 = m._m20;
    _m03 = m._m30;

    _m10 = m._m01;
    _m11 = m._m11;
    _m12 = m._m21;
    _m13 = m._m31;

    _m20 = m._m02;
    _m21 = m._m12;
    _m22 = m._m22;
    _m23 = m._m32;

    _m31 = m._m13;
    _m32 = m._m23;
    _m30 = m._m03;
    _m33 = m._m33;

}

float EngineMath::Matrix4x4::getDeterminant()
{
    return
    _m03  * _m12  * _m21  * _m30  - _m02  * _m13  * _m21  * _m30  -
    _m03  * _m11  * _m22  * _m30  + _m01  * _m13  * _m22  * _m30  +
    _m02  * _m11  * _m23  * _m30  - _m01  * _m12  * _m23  * _m30  -
    _m03  * _m12  * _m20  * _m31  + _m02  * _m13  * _m20  * _m31  +
    _m03  * _m10  * _m22  * _m31  - _m00  * _m13  * _m22  * _m31  -
    _m02  * _m10  * _m23  * _m31  + _m00  * _m12  * _m23  * _m31  +
    _m03  * _m11  * _m20  * _m32  - _m01  * _m13  * _m20  * _m32  -
    _m03  * _m10  * _m21  * _m32  + _m00  * _m13  * _m21  * _m32  +
    _m01  * _m10  * _m23  * _m32  - _m00  * _m11  * _m23  * _m32  -
    _m02  * _m11  * _m20  * _m33  + _m01  * _m12  * _m20  * _m33  +
    _m02  * _m10  * _m21  * _m33  - _m00  * _m12  * _m21  * _m33  -
    _m01  * _m10  * _m22  * _m33  + _m00  * _m11  * _m22  * _m33 ;
}

float EngineMath::Matrix4x4::trace()
{
    return  _m00 + _m11 + _m22 + _m33;
}

void EngineMath::Matrix4x4::SetRow(int row, Vector4 value)
{
    switch (row)
    {
    case 0:
        _m00 = value.x;
        _m01 = value.y;
        _m02 = value.z;
        _m03 = value.w;
        break;
    case 1:
        _m10 = value.x;
        _m11 = value.y;
        _m12 = value.z;
        _m13 = value.w;
        break;
    case 2:
        _m20 = value.x;
        _m21 = value.y;
        _m22 = value.z;
        _m23 = value.w;
        break;
    case 3:
        _m30 = value.x;
        _m31 = value.y;
        _m32 = value.z;
        _m33 = value.w;
        break;
    default:
        throw std::exception("Wrong row index");
        break;
    }
    return;

}
Vector4 EngineMath::Matrix4x4::TransformVector(Vector4 v)
{
    Vector4 out;
    out.x = v.x * _m00 + v.y * _m01 + v.z * _m02 + v.w * _m03;
    out.y = v.x * _m10 + v.y * _m11 + v.z * _m12 + v.w * _m13;
    out.z = v.x * _m20 + v.y * _m21 + v.z * _m22 + v.w * _m23;
    out.w = v.x * _m30 + v.y * _m31 + v.z * _m32 + v.w * _m33;

    return out;
}

Matrix4x4 EngineMath::Matrix4x4::Inversed()
{
    Matrix4x4 This = *this;
    Matrix4x4 Squared = This * This;
    Matrix4x4 Cubed = Squared * This;
    Matrix4x4 A, B, C, D;
    A = Identity() * (0.16666666666666666666666666666667f * (float)(pow(This.trace(), 3) - 3 * (float)This.trace() * Squared.trace() + 2 * (float)Cubed.trace()));
    B = This * (0.5f * (float)(pow(This.trace(), 2) - Squared.trace()));
    C = Squared * This.trace();
    D = (A - B + C - Cubed) * (1 / This.getDeterminant());

    return D;
}

Matrix4x4 EngineMath::Matrix4x4::operator+(Matrix4x4 m)
{
    return Matrix4x4(
        _m00 + m._m00, _m01 + m._m01, _m02 + m._m02, _m03 + m._m03,
        _m10 + m._m10, _m11 + m._m11, _m12 + m._m12, _m13 + m._m13,
        _m20 + m._m20, _m21 + m._m21, _m22 + m._m22, _m23 + m._m23,
        _m30 + m._m30, _m31 + m._m31, _m32 + m._m32, _m33 + m._m33);
}
Matrix4x4 EngineMath::Matrix4x4::operator-(Matrix4x4 m)
{
    return Matrix4x4(
        _m00 - m._m00, _m01 - m._m01, _m02 - m._m02, _m03 - m._m03,
        _m10 - m._m10, _m11 - m._m11, _m12 - m._m12, _m13 - m._m13,
        _m20 - m._m20, _m21 - m._m21, _m22 - m._m22, _m23 - m._m23,
        _m30 - m._m30, _m31 - m._m31, _m32 - m._m32, _m33 - m._m33);

}
Matrix4x4 EngineMath::Matrix4x4::operator*(float c)
{
    return Matrix4x4(
        _m00 * c, _m01 * c, _m02 * c, _m03 * c,
        _m10 * c, _m11 * c, _m12 * c, _m13 * c,
        _m20 * c, _m21 * c, _m22 * c, _m23 * c,
        _m30 * c, _m31 * c, _m32 * c, _m33 * c);
}
Matrix4x4 EngineMath::Matrix4x4::operator/(float c)
{
    return Matrix4x4(GetRow(0) / c, GetRow(2) / c, GetRow(3) / c, GetRow(4) / c);
}
Matrix4x4 EngineMath::Matrix4x4::operator*(Matrix4x4 m)
{
    

    Vector4 A, A1, A2, A3, B, B1, B2, B3;;
    A = Vector4(_m00, _m01, _m02, _m03);
    A1 = Vector4(_m10, _m11, _m12, _m13);
    A2 = Vector4(_m20, _m21, _m22, _m23);
    A3 = Vector4(_m30, _m31, _m32, _m33);
     
    B = Vector4(m._m00, m._m10, m._m20, m._m30);
    B1 = Vector4(m._m01, m._m11, m._m21, m._m31);
    B2 = Vector4(m._m02, m._m12, m._m22, m._m32);
    B3 = Vector4(m._m03, m._m13, m._m23, m._m33);

    return Matrix4x4(
        A * B, A * B1, A * B2, A * B3,
        A1 * B, A1 * B1, A1 * B2, A1 * B3,
        A2 * B, A2 * B1, A2 * B2, A2 * B3,
        A3 * B, A3 * B1, A3 * B2, A3 * B3
    );
}
void EngineMath::Matrix4x4::operator+=(Matrix4x4 m)
{
    SetRow(0, GetRow(0) + m.GetRow(0));
    SetRow(1, GetRow(1) + m.GetRow(1));
    SetRow(2, GetRow(2) + m.GetRow(2));
    SetRow(3, GetRow(3) + m.GetRow(3));
    return;

}
void EngineMath::Matrix4x4::operator-=(Matrix4x4 m)
{
    SetRow(0, GetRow(0) - m.GetRow(0));
    SetRow(1, GetRow(1) - m.GetRow(1));
    SetRow(2, GetRow(2) - m.GetRow(2));
    SetRow(3, GetRow(3) - m.GetRow(3));
    return;
}
void EngineMath::Matrix4x4::operator*=(float c)
{
    SetRow(0, GetRow(0) * c);
    SetRow(1, GetRow(1) * c);
    SetRow(2, GetRow(2) * c);
    SetRow(3, GetRow(3) * c);
    return;
}
void EngineMath::Matrix4x4::operator/=(float c)
{
    SetRow(0, GetRow(0) / c);
    SetRow(1, GetRow(1) / c);
    SetRow(2, GetRow(2) / c);
    SetRow(3, GetRow(3) / c);
    return;

}
void EngineMath::Matrix4x4::operator=(Matrix4x4 m)
{
    _m00 = m._m00;
    _m01 = m._m01;
    _m02 = m._m02;
    _m03 = m._m03;

    _m10 = m._m10;
    _m11 = m._m11;
    _m12 = m._m12;
    _m13 = m._m13;

    _m20 = m._m20;
    _m21 = m._m21;
    _m22 = m._m22;
    _m23 = m._m23;

    _m30 = m._m30;
    _m31 = m._m31;
    _m32 = m._m32;
    _m33 = m._m33;
    return;
    
}
Vector4 EngineMath::Matrix4x4::operator*(Vector4 v)
{
    Vector4 out;
    out.x = v.x * _m00 + v.y * _m01 + v.z * _m02 + v.w * _m03;
    out.y = v.x * _m10 + v.y * _m11 + v.z * _m12 + v.w * _m13;
    out.z = v.x * _m20 + v.y * _m21 + v.z * _m22 + v.w * _m23;
    out.w = v.x * _m30 + v.y * _m31 + v.z * _m32 + v.w * _m33;

    return out;
}

bool EngineMath::Matrix4x4::operator==(Matrix4x4 m)
{
    return 
        (_m00 == m._m00)&& (_m01 == m._m01)&& (_m02 == m._m02)&& (_m03 == m._m03)&&
        (_m10 == m._m10)&& (_m11 == m._m11)&& (_m12 == m._m12)&& (_m13 == m._m13)&&
        (_m20 == m._m20)&& (_m21 == m._m21)&& (_m22 == m._m22)&& (_m23 == m._m23)&& 
        (_m30 == m._m30)&& (_m31 == m._m31)&& (_m32 == m._m32)&& (_m33 == m._m33);
}

/*/EngineMath::Matrix4x4::operator DirectX::XMFLOAT4X4()
{
    return DirectX::XMFLOAT4X4(
        _m00, _m01, _m02, _m03,
        _m10, _m11, _m12, _m13,
        _m20, _m21, _m22, _m23,
        _m30, _m31, _m32, _m33);
}*/

Vector4 EngineMath::Matrix4x4::operator[](int index)
{
    return GetRow(index);
}

Vector4 EngineMath::Matrix4x4::GetRow(int index)
{
    switch (index)
    {
    case 0:
        return Vector4(_m00, _m01, _m02, _m03);
        break;
    case 1:
        return Vector4(_m10, _m11, _m12, _m13);
        break;
    case 2:
        return Vector4(_m20, _m21, _m22, _m23);
        break;
    case 3:
        return Vector4(_m30, _m31, _m32, _m33);
        break;
    default:
        throw std::exception("Wrong row index");
        break;
    }
}


