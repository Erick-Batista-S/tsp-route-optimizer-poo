#ifndef ROUTEVISUALIZER_H
#define ROUTEVISUALIZER_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <vector>
#include <memory>
#include "TSPClasses.h"

/**
 * @brief Widget para visualização animada da construção de rotas TSP
 * 
 * Esta classe especializada demonstra a animação passo-a-passo
 * da construção de uma solução TSP, permitindo visualizar como
 * diferentes algoritmos constroem suas soluções.
 * 
 * Conceitos POO demonstrados:
 * - Herança: RouteVisualizer herda de QWidget
 * - Composição: Contém Route e animations
 * - Encapsulamento: Estado interno da animação
 * - Template/STL: Uso de std::vector para pontos
 */
class RouteVisualizer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double animationProgress READ animationProgress WRITE setAnimationProgress)

public:
    explicit RouteVisualizer(QWidget *parent = nullptr);
    ~RouteVisualizer();
    
    // Interface principal
    void setRoute(std::shared_ptr<Route> route);
    void startAnimation();
    void stopAnimation();
    void resetAnimation();
    
    // Configurações da animação
    void setAnimationDuration(int durationMs);
    void setAnimationEasing(QEasingCurve::Type easing);
    void setShowProgress(bool show);
    
    // Estado da animação
    bool isAnimating() const;
    double animationProgress() const;
    void setAnimationProgress(double progress);

signals:
    void animationStarted();
    void animationFinished();
    void progressChanged(double progress);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateAnimation();
    void onAnimationFinished();

private:
    // Métodos de desenho
    void drawRoute(QPainter& painter);
    void drawAnimatedSegment(QPainter& painter, int fromIndex, int toIndex, double progress);
    void drawRouteProgress(QPainter& painter);
    void drawStatistics(QPainter& painter);
    
    // Métodos de transformação
    QPointF worldToScreen(const Point& point) const;
    QRectF calculateBounds() const;
    void updateTransform();
    
    // Dados do modelo
    std::shared_ptr<Route> m_route;
    
    // Estado da animação
    QPropertyAnimation* m_animation;
    QTimer* m_updateTimer;
    double m_animationProgress;
    bool m_isAnimating;
    int m_animationDuration;
    QEasingCurve::Type m_easingType;
    bool m_showProgress;
    
    // Estado da visualização
    double m_scale;
    QPointF m_offset;
    QRectF m_viewport;
    
    // Configurações visuais
    struct AnimationStyle {
        QColor backgroundColor;
        QColor routeColor;
        QColor animatedColor;
        QColor progressColor;
        QColor textColor;
        QPen routePen;
        QPen animatedPen;
        QBrush pointBrush;
        QFont statisticsFont;
        int pointRadius;
        int routeWidth;
        int animatedWidth;
    } m_style;
    
    void initializeStyle();
};

#endif // ROUTEVISUALIZER_H