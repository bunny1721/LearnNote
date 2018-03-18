cocos2dx中结合OpenGL实现自定义精灵类详解（自定义顶点坐标以及纹理坐标）
============================================================
>>cocos2dx是基于OpenGL ES所开发的游戏引擎,因此通过一些简单的修改可以实现自定义Sprite渲染.<br>然后呢因为不是什么高深知识（本人也是菜鸟新手高深知识神马的不存在的）希望dalao勿喷。<br>

初始化着色器
---------
cocos中已经自定义很多种类的着色器代码段为了简便使用cocos原生着色器渲染进行案例的使用。<br>
```c++
setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
```
GLProgram::SHADER_NAME_POSITION_TEXTURE是创建shader 里面包含Position以及Texture的属性<br>
设置自定义顶点纹理坐标
-----------
```c++
m_vertices = new float[9] 
	{
		0.f, 0.f, 0.f,
		_contentSize.width, 0.f, 0.f, 
		_contentSize.width / 2.f, _contentSize.height, 0.f
	};
	m_texCoords = new float[6]
	{
		0.f, 1.0f,
		1.f, 1.0f,
		0.5f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) *9, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*9, sizeof(float) *6, m_texCoords);
	glEnableVertexAttribArray(kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) *9));
  ```
  在以上代码中定义了顶点坐标和纹理坐标数组<br>
  使用opengl中的顶点数组缓存VAO记录顶点缓存数据VBO执行分配空间 初始化属性的操作<br>
  
  绘制方法中进行绘制
  -------
  ```c++
  getGLProgram()->use();
	glBindVertexArray(VAO);
	getGLProgram()->setUniformsForBuiltins(transform);
	GL::bindTexture2D(m_texture->getName());
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
  ```
  在上述代码中可以看到setUniformsForBuiltins(transform);这是cocos设置MVP变换<br>
  MVP即模型、视图和投影变换处理后可最终显示到游戏窗空中（有兴趣的童鞋可以查阅相关文档资料）<br>
  
  下面是完整封装过后的自定义Sprite类
  -----------------------------
  MySprite.h文件：<br>
  ```c++
  #pragma once
  #include "cocos2d.h"
  USING_NS_CC;
  class MySprite :public Node
  {
  public:
    MySprite();
    ~MySprite();
    static MySprite* createWithFile(std::string topFile);
    bool initWithFile(std::string topFile);
    bool initWithTexture(Texture2D* texture);
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;
    void onDraw(const Mat4& transform, uint32_t flags);
  protected:
    void initData();
    void update(float _dt);
  private:
    CustomCommand m_customCommand;
    float* m_vertices;
    float* m_texCoords;
    Texture2D* m_texture;
    int m_vertexBufferSize;
    int m_texBufferSize;
    unsigned int VAO ,VBO;
  };<br>
  ```
  
  
  MySprite.cpp文件：<br>
  ```c++
  #include "MySprite.h"
  MySprite::MySprite():
    m_vertices(nullptr),
    m_texCoords(nullptr)
  {
  }
  MySprite::~MySprite()
  {
    CC_SAFE_DELETE_ARRAY(m_vertices);
    CC_SAFE_DELETE_ARRAY(m_texCoords);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }
  MySprite* MySprite::createWithFile(std::string topFile)
  {
    MySprite* sp = new MySprite();
    if (sp && sp->initWithFile(topFile))
    {
      sp->autorelease();
    }
    else
    {
      CC_SAFE_DELETE(sp);
    }
    return sp;
  }
  bool MySprite::initWithFile(std::string topFile)
  {
    return initWithTexture(TextureCache::getInstance()->addImage(topFile));
  }

  bool MySprite::initWithTexture(Texture2D * texture)
  {
    m_texture = texture;
    if (!m_texture)return false;
    m_texture->retain();
    setContentSize(m_texture->getContentSize());
    setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    this->initData();
    scheduleUpdate();
    return true;
  }
  void MySprite::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
  {
    m_customCommand.init(_globalZOrder);
    m_customCommand.func = CC_CALLBACK_0(MySprite::onDraw, this, transform, flags);
    renderer->addCommand(&m_customCommand);
  }
  void MySprite::onDraw(const Mat4 & transform, uint32_t flags)
  {
    getGLProgram()->use();
    glBindVertexArray(VAO);
    getGLProgram()->setUniformsForBuiltins(transform);
    GL::bindTexture2D(m_texture->getName());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
  }
  void MySprite::initData()
  {
    m_vertices = new float[9] 
    {
      0.f, 0.f, 0.f,
      _contentSize.width, 0.f, 0.f, 
      _contentSize.width / 2.f, _contentSize.height, 0.f
    };
    m_texCoords = new float[6]
    {
      0.f, 1.0f,
      1.f, 1.0f,
      0.5f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) *9, m_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*9, sizeof(float) *6, m_texCoords);
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) *9));
  }
  void MySprite::update(float _dt)
  {

  }
  ```
  
  PS:因为cocos渲染是加入渲染队列之后统一循环渲染，因此我们要保证自定义精灵类可以加入cocos引擎渲染队列需要重载Node的draw函数，创建CustomCommand变量并添加至cocos渲染队列<br>
  
  最终输出结果如下图所示：
  -----------
  ![](https://github.com/bunny1721/LearnNote/blob/master/Cocos2d-XNote/res/image0.png)<br>
  
  通过上述代码修改对应的顶点纹理坐标即可实现很多特殊效果（详细资料请出门左转咨询各种数学算法(*^▽^*)）
  
  
  
