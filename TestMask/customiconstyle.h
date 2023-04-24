// QMenu的大图标定义
#ifndef CUSTOMICONSTYLE_H
#define CUSTOMICONSTYLE_H

#include <QProxyStyle>

class CustomIconStyle : public QProxyStyle
{
    Q_OBJECT

public:
    ///
    /// \brief CustomIconStyle
    /// 设置自定义风格界面，主要用于设置菜单条目上的图标大小
    ///
    CustomIconStyle();
    ~CustomIconStyle();

    ///
    /// \brief SetCustomSize 设置图标大小
    ///
    void SetCustomSize(int);

    ///
    /// \brief pixelMetric 返回像素度量的大小
    /// \param metric 像素度量
    /// \param option 风格选项
    /// \param widget 正在绘制的小部件
    /// \return
    ///
    virtual int pixelMetric(
        PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;

private:
    int mSize;
};

#endif // CUSTOMICONSTYLE_H
