#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include <QRectF>
#include <QTimer>
#include <vector>
#include <memory>
#include "TSPClasses.h"

/**
 * @brief Widget customizado para visualização interativa do grafo TSP
 * 
 * Esta classe herda de QWidget e implementa desenho customizado para:
 * - Visualizar pontos/cidades como círculos
 * - Desenhar rotas como linhas conectando pontos
 * - Permitir adicionar pontos através de cliques do mouse
 * - Suportar zoom e pan para navegação
 * - Animação da solução sendo construída
 * 
 * Conceitos POO demonstrados:
 * - Herança: GraphView herda de QWidget
 * - Polimorfismo: Override dos métodos de evento do Qt
 * - Encapsulamento: Estado interno do componente
 * - Composição: Contém objetos Point, Graph, Route
 */
class GraphView : public QWidget
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = nullptr);
    ~GraphView();
    
    // Interface pública
    void setGraph(std::shared_ptr<Graph> graph);
    void setRoute(std::shared_ptr<Route> route);
    void clearGraph();
    void addPoint(const Point& point);
    void removeLastPoint();
    
    // Controles de visualização
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void fitToWindow();
    
    // Configurações visuais
    void setShowPointLabels(bool show);
    void setShowDistances(bool show);
    void setAnimateRoute(bool animate);

signals:
    // Sinais emitidos para comunicação com MainWindow
    void pointAdded(const Point& point);
    void pointRemoved(int index);
    void graphChanged();

protected:
    // Sobrescrita de métodos de evento (polimorfismo)
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void animationStep();

private:
    // Métodos de desenho
    void drawBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawPoints(QPainter& painter);
    void drawRoute(QPainter& painter);
    void drawPointLabels(QPainter& painter);
    void drawDistances(QPainter& painter);
    void drawAnimatedRoute(QPainter& painter);
    
    // Métodos de transformação de coordenadas
    QPointF worldToScreen(const Point& point) const;
    QPointF worldToScreen(double x, double y) const;
    QPointF screenToWorld(const QPointF& screenPoint) const;
    Point screenToWorldPoint(const QPointF& screenPoint) const;
    QRectF getWorldBounds() const;
    void updateTransform();
    
    // Métodos auxiliares
    QString formatDistance(double distance) const;
    QColor getPointColor(int index) const;
    QColor getRouteColor() const;
    
    // Dados do modelo
    std::shared_ptr<Graph> m_graph;
    std::shared_ptr<Route> m_route;
    
    // Estado da visualização
    double m_scale;
    QPointF m_offset;
    QRectF m_viewport;
    bool m_isDragging;
    QPointF m_lastMousePos;
    
    // Configurações visuais
    bool m_showPointLabels;
    bool m_showDistances;
    bool m_animateRoute;
    
    // Animação
    QTimer* m_animationTimer;
    int m_animationStep;
    int m_maxAnimationSteps;
    
    // Estilos visuais
    struct VisualStyle {
        QColor backgroundColor;
        QColor gridColor;
        QColor pointColor;
        QColor pointSelectedColor;
        QColor routeColor;
        QColor routeAnimatedColor;
        QColor textColor;
        QPen pointPen;
        QPen routePen;
        QPen gridPen;
        QBrush pointBrush;
        QFont labelFont;
        QFont distanceFont;
        int pointRadius;
        int routeWidth;
    } m_style;
    
    void initializeStyle();
    void updateStyle();
};

#endif // GRAPHVIEW_H