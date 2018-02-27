
四元数的学习以及unity中的应用实现
=============================
>>最近在学习OpenGL空间变换时，学习使用了欧拉角旋转，后经过各方面的资料学习发现，欧拉角存在万向节锁问题（关于此问题再次不予讨论）。使用四元数可以解决万向节锁问题，在学习四元数的过程中有不少心得体会在此做下笔记并且与大家进行分享交流。 (本文仅讨论四元数旋转的计算过程，所以使用unity引擎作为辅助运算展示的引擎，不使用OpenGL原生代码，以免给大家造成不必要的困扰)<br>
  
四元数简介
--------
>>四元数（Quaternions）是由爱尔兰数学家哈密顿(William Rowan Hamilton,1805-1865）在1843年发明的数学概念。在3D空间变换中有平移、缩放和旋转，其中旋转有大家很熟悉的欧拉角旋转和很抽象的四元数旋转。四元数是简单的超复数。 复数是由实数加上虚数单位 i 组成，其中i^2 = -1。 相似地，四元数都是由实数加上三个虚数单位 i、j、k 组成，而且它们有如下的关系：<br> i^2 = j^2 = k^2 = -1， <br> i^0 = j^0 = k^0 = 1 ,<br> 每个四元数都是 1、i、j 和 k 的线性组合，即是四元数一般可表示为<br> >>a + bk+ cj + di，<br> >>其中a、b、c 、d是实数。
  
四元数基础
---------
>>一个四元数可以表示为q = w + xi + yj + zk，大家可能会有疑问，四元数在三维旋转中是如何结合使用的呢，在此大家可以简单的理解为四元数q = （(x,y,z),w)，其中（x, y ,z）即三维向量，w是实数。<br>
在三维空间旋转中，四元数可以理解为点（x,y,z）绕旋转轴Rotation axis <br>(四元数表示的旋转轴（u\*sin(angle/2) , cos(angle/2) ,其中u是单位向量 angle是旋转角度)旋转一定角度。<br>
![](https://github.com/bunny1721/LearnNote/blob/master/Learn%20quaternion/res/image1.png)<br>
在上面提到四元数q = （(x,y,z)sin(θ/2), cos(θ/2)）来执行一个旋转，点P为要旋转的点绕四元数所表示的旋转轴q来进行旋转，首先我们需要把P拓展为四元数p=(P,0)。计算后得到的新的四元数p' = (P',0)。那么，旋转后新的点对应的四元数（当然这个计算而得的四元数的实部为0，虚部系数就是新的坐标）为：<br> p′=qpq−1 <br>
其中：<br>
q=(cosθ2, (x,y,z)sinθ2) ，q−1=q∗N(q)，由于u是单位向量，因此<br>
N(q)=1，即q−1=q∗。右边表达式包含了四元数乘法。相关的定义如下：<br>
  ● 四元数乘法：q1q2=(v1→×v2→+w1v2→+w2v1→,w1w2−v1→⋅v2→) (v1→为向量v1)<br>
  ● 共轭四元数：q∗=(−v⃗ ,w)<br>
  ● 四元数的模：N(q) = √(x^2 + y^2 + z^2 +w^2)，即四元数到原点的距离<br>
  ● 四元数的逆：q−1=q∗N(q)<br>
  p'的计算过程根据上方的四元数运算公式可进行过程计算（其中涉及的向量点积叉积运算此处不再赘述）<br>
  下面贴上代码（unityc#版本）:<br>
  自定义四元数类:<br>
  ```c#
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class MQuaternions
{

    public float _x;
    public float _y;
    public float _z;
    public float _w;
    public float _nx;
    public float _ny;
    public float _nz;
    public float _nw;

    MQuaternions()
    {

    }
    public MQuaternions(float x, float y, float z, float w)
    {

        _x = x;
	_y = y;
	_z = z;
	_w = w;

        unitary();
    }
    public MQuaternions AxisRotation(MQuaternions _axis, float _angle)
    {
        MQuaternions result = new MQuaternions();
        MQuaternions q = new MQuaternions();
        MQuaternions p = new MQuaternions();
        MQuaternions qN = new MQuaternions();
        float radian = (_angle * 3.1415926f) / 180.0f;
        q._x = _axis._nx * Mathf.Sin(radian);
        q._y = _axis._ny * Mathf.Sin(radian);
        q._z = _axis._nz * Mathf.Sin(radian);
        q._w = Mathf.Cos(radian);

        qN._x = -q._x;
        qN._y = -q._y;
        qN._z = -q._z;
        qN._w = q._w;

        p._x = _x;
        p._y = _y;
        p._z = _z;
        p._w = _w;

        result = q.Ride(p).Ride(qN);
        return result;

    }
    public MQuaternions Ride(MQuaternions _target)
    {
        float x1 = _y * _target._z - _z * _target._y;
        float y1 = _z * _target._x - _x * _target._z;
        float z1 = _x * _target._y - _y * _target._x;

        float x2 = _w * _target._x;
        float y2 = _w * _target._y;
        float z2 = _w * _target._z;

        float x3 = _target._w * _x;
        float y3 = _target._w * _y;
        float z3 = _target._w * _z;

        float w1 = _w * _target._w;
        float w2 = _x * _target._x + _y * _target._y + _z * _target._z;

        MQuaternions _result = new MQuaternions();
        _result._x = x1 + x2 + x3;
        _result._y = y1 + y2 + y3;
        _result._z = z1 + z2 + z3;
        _result._w = w1 - w2;
        return _result;
    }
    public void unitary()
    {
        float divisor = Mathf.Sqrt(_x * _x + _y * _y + _z * _z);
        _nx = _x / divisor;
        _ny = _y / divisor;
        _nz = _z / divisor;
        _nw = _w;
    }
}
``` <br>

测试类：<br>
```c#
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class test : MonoBehaviour {

    public GameObject aix;
    public float angle;

   // Vector3 resetPostion = new Vector3();
    //Vector3 disVec;
    // Use this for initialization
    void Start () {
       // resetPostion = transform.position;
        
    }

    // Update is called once per frame
    void Update () {
       MQuaternions q1 = new MQuaternions(transform.position.x, transform.position.y, transform.position.z, 0);
       MQuaternions q2 = new MQuaternions(aix.transform.position.x, aix.transform.position.y, aix.transform.position.z, 0);
       MQuaternions result = q1.AxisRotation(q2, angle);
       transform.position = new Vector3(result._x, result._y, result._z);

        if (Input.GetKey(KeyCode.UpArrow))
        {
            aix.transform.position += new Vector3(0, 0.1f, 0);
        }
        else if (Input.GetKey(KeyCode.DownArrow))
        {
            aix.transform.position += new Vector3(0, -0.1f, 0);
        }
        else if (Input.GetKey(KeyCode.LeftArrow))
        {
            aix.transform.position += new Vector3(-0.1f, 0, 0);
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            aix.transform.position += new Vector3(0.1f, 0, 0);
        }
    }
}
``` <br>

使用方法将test类挂在需要旋转的球体上，另一个球体位置代表旋转轴，如下图所示：<br>
![](https://github.com/bunny1721/LearnNote/blob/master/Learn%20quaternion/res/image2.png)<br>

好了上面就是关于四元数旋转的计算过程的实现，另有c++版本代码 在此项目目录下。

