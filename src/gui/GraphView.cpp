#include "GraphView.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include <QRectF>
#include <QDebug>
#include <cmath>
#include <algorithm>

// Reusar as classes Point, Graph, Route já definidas na MainWindow
// Por simplicidade, vou redefini-las aqui (em um projeto real, estariam em headers separados)

GraphView::GraphView(QWidget *parent)
    : QWidget(parent)
    , m_scale(1.0)
    , m_offset(0, 0)
    , m_isDragging(false)
    , m_showPointLabels(true)
    , m_showDistances(false)
    , m_animateRoute(true)
    , m_animationStep(0)
    , m_maxAnimationSteps(50)
{
    setMinimumSize(400, 300);
    setMouseTracking(true);
    
    // Configurar timer de animação
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &GraphView::animationStep);
    
    // Inicializar estilo visual
    initializeStyle();
    
    // Configurar viewport inicial
    m_viewport = QRectF(-200, -200, 400, 400);
    updateTransform();
}

GraphView::~GraphView() = default;

void GraphView::setGraph(std::shared_ptr<Graph> graph)
{
    m_graph = graph;
    
    if (m_graph && m_graph->getSize() > 0) {
        fitToWindow();
    }
    
    update();
    emit graphChanged();
}

void GraphView::setRoute(std::shared_ptr<Route> route)
{
    m_route = route;
    
    if (m_animateRoute && m_route) {
        m_animationStep = 0;
        m_animationTimer->start(50); // 20 FPS
    }
    
    update();
}

void GraphView::clearGraph()
{
    m_graph.reset();
    m_route.reset();
    m_animationTimer->stop();
    m_animationStep = 0;
    
    update();
    emit graphChanged();
}

void GraphView::addPoint(const Point& point)
{
    emit pointAdded(point);
    update();
}

void GraphView::removeLastPoint()
{
    if (m_graph && m_graph->getSize() > 0) {
        // Implementação seria necessária na classe Graph
        emit pointRemoved(m_graph->getSize() - 1);
        update();
    }
}

void GraphView::zoomIn()
{
    m_scale *= 1.2;
    updateTransform();
    update();
}

void GraphView::zoomOut()
{
    m_scale /= 1.2;
    updateTransform();
    update();
}

void GraphView::resetZoom()
{
    m_scale = 1.0;
    m_offset = QPointF(0, 0);
    updateTransform();
    update();
}

void GraphView::fitToWindow()
{
    if (!m_graph || m_graph->getSize() == 0) {
        resetZoom();
        return;
    }
    
    QRectF bounds = getWorldBounds();
    if (bounds.isEmpty()) {
        resetZoom();
        return;
    }
    
    // Adicionar margem
    bounds = bounds.adjusted(-20, -20, 20, 20);
    
    // Calcular escala para caber na janela
    double scaleX = width() / bounds.width();
    double scaleY = height() / bounds.height();
    m_scale = std::min(scaleX, scaleY) * 0.9; // 90% para margem
    
    // Centralizar
    m_offset = -bounds.center() * m_scale + QPointF(width() / 2.0, height() / 2.0);
    
    updateTransform();
    update();
}

void GraphView::setShowPointLabels(bool show)
{
    m_showPointLabels = show;
    update();
}

void GraphView::setShowDistances(bool show)
{
    m_showDistances = show;
    update();
}

void GraphView::setAnimateRoute(bool animate)
{
    m_animateRoute = animate;
    if (!animate) {
        m_animationTimer->stop();
        m_animationStep = m_maxAnimationSteps;
        update();
    }
}

void GraphView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Desenhar componentes na ordem correta
    drawBackground(painter);
    drawGrid(painter);
    
    if (m_graph) {
        if (m_route && m_animateRoute) {
            drawAnimatedRoute(painter);
        } else if (m_route) {
            drawRoute(painter);
        }
        
        drawPoints(painter);
        
        if (m_showPointLabels) {
            drawPointLabels(painter);
        }
        
        if (m_showDistances) {
            drawDistances(painter);
        }
    }
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Point worldPoint = screenToWorldPoint(event->pos());
        
        // Criar nome automático
        int pointCount = m_graph ? m_graph->getSize() : 0;
        std::string pointName = "P" + std::to_string(pointCount + 1);
        
        Point newPoint(worldPoint.getX(), worldPoint.getY(), pointName);
        addPoint(newPoint);
        
        m_lastMousePos = event->pos();
    } else if (event->button() == Qt::RightButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void GraphView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        QPointF delta = event->pos() - m_lastMousePos;
        m_offset += delta;
        m_lastMousePos = event->pos();
        updateTransform();
        update();
    }
}

void GraphView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    
    if (event->angleDelta().y() > 0) {
        m_scale *= scaleFactor;
    } else {
        m_scale /= scaleFactor;
    }
    
    // Limitar zoom
    m_scale = std::max(0.1, std::min(10.0, m_scale));
    
    updateTransform();
    update();
}

void GraphView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    updateTransform();
}

void GraphView::animationStep()
{
    if (m_animationStep < m_maxAnimationSteps) {
        m_animationStep++;
        update();
    } else {
        m_animationTimer->stop();
    }
}

void GraphView::drawBackground(QPainter& painter)
{
    painter.fillRect(rect(), m_style.backgroundColor);
}

void GraphView::drawGrid(QPainter& painter)
{
    painter.setPen(m_style.gridPen);
    
    // Determinar espaçamento da grade baseado no zoom
    double gridSpacing = 50.0;
    while (gridSpacing * m_scale < 20) gridSpacing *= 2;
    while (gridSpacing * m_scale > 100) gridSpacing /= 2;
    
    // Desenhar linhas verticais
    double startX = std::floor(m_viewport.left() / gridSpacing) * gridSpacing;
    for (double x = startX; x <= m_viewport.right(); x += gridSpacing) {
        QPointF p1 = worldToScreen(x, m_viewport.top());
        QPointF p2 = worldToScreen(x, m_viewport.bottom());
        painter.drawLine(p1, p2);
    }
    
    // Desenhar linhas horizontais
    double startY = std::floor(m_viewport.top() / gridSpacing) * gridSpacing;
    for (double y = startY; y <= m_viewport.bottom(); y += gridSpacing) {
        QPointF p1 = worldToScreen(m_viewport.left(), y);
        QPointF p2 = worldToScreen(m_viewport.right(), y);
        painter.drawLine(p1, p2);
    }
}

void GraphView::drawPoints(QPainter& painter)
{
    if (!m_graph) return;
    
    painter.setPen(m_style.pointPen);
    painter.setBrush(m_style.pointBrush);
    
    for (size_t i = 0; i < m_graph->getSize(); ++i) {
        const auto& point = m_graph->getPoint(i);
        QPointF screenPos = worldToScreen(point);
        
        QColor color = getPointColor(i);
        painter.setBrush(QBrush(color));
        
        painter.drawEllipse(screenPos, m_style.pointRadius, m_style.pointRadius);
    }
}

void GraphView::drawRoute(QPainter& painter)
{
    if (!m_route || m_route->getSize() < 2) return;
    
    painter.setPen(QPen(getRouteColor(), m_style.routeWidth));
    
    const auto& points = m_route->getPoints();
    
    // Desenhar segmentos da rota
    for (size_t i = 0; i < points.size() - 1; ++i) {
        QPointF p1 = worldToScreen(points[i]);
        QPointF p2 = worldToScreen(points[i + 1]);
        painter.drawLine(p1, p2);
    }
    
    // Fechar o circuito (voltar ao ponto inicial)
    if (points.size() > 2) {
        QPointF p1 = worldToScreen(points.back());
        QPointF p2 = worldToScreen(points.front());
        painter.drawLine(p1, p2);
    }
}

void GraphView::drawPointLabels(QPainter& painter)
{
    if (!m_graph) return;
    
    painter.setPen(QPen(m_style.textColor));
    painter.setFont(m_style.labelFont);
    
    for (size_t i = 0; i < m_graph->getSize(); ++i) {
        const auto& point = m_graph->getPoint(i);
        QPointF screenPos = worldToScreen(point);
        
        QString label = QString::fromStdString(point.getName());
        QRectF textRect(screenPos.x() + m_style.pointRadius + 5, 
                       screenPos.y() - 10, 50, 20);
        
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, label);
    }
}

void GraphView::drawDistances(QPainter& painter)
{
    if (!m_route || m_route->getSize() < 2) return;
    
    painter.setPen(QPen(m_style.textColor));
    painter.setFont(m_style.distanceFont);
    
    const auto& points = m_route->getPoints();
    
    for (size_t i = 0; i < points.size() - 1; ++i) {
        QPointF p1 = worldToScreen(points[i]);
        QPointF p2 = worldToScreen(points[i + 1]);
        QPointF midPoint = (p1 + p2) / 2;
        
        double distance = points[i].distanceTo(points[i + 1]);
        QString distText = formatDistance(distance);
        
        painter.drawText(midPoint, distText);
    }
}

void GraphView::drawAnimatedRoute(QPainter& painter)
{
    if (!m_route || m_route->getSize() < 2) return;
    
    const auto& points = m_route->getPoints();
    double progress = double(m_animationStep) / m_maxAnimationSteps;
    int visibleSegments = int(progress * points.size());
    
    // Desenhar segmentos completos
    painter.setPen(QPen(getRouteColor(), m_style.routeWidth));
    for (int i = 0; i < visibleSegments && i < int(points.size()) - 1; ++i) {
        QPointF p1 = worldToScreen(points[i]);
        QPointF p2 = worldToScreen(points[i + 1]);
        painter.drawLine(p1, p2);
    }
    
    // Desenhar segmento parcial
    if (visibleSegments < int(points.size()) - 1) {
        double segmentProgress = (progress * points.size()) - visibleSegments;
        QPointF p1 = worldToScreen(points[visibleSegments]);
        QPointF p2 = worldToScreen(points[visibleSegments + 1]);
        QPointF partialEnd = p1 + (p2 - p1) * segmentProgress;
        
        painter.setPen(QPen(m_style.routeAnimatedColor, m_style.routeWidth + 2));
        painter.drawLine(p1, partialEnd);
    }
    
    // Fechar circuito no final da animação
    if (progress >= 0.95 && points.size() > 2) {
        QPointF p1 = worldToScreen(points.back());
        QPointF p2 = worldToScreen(points.front());
        painter.setPen(QPen(getRouteColor(), m_style.routeWidth));
        painter.drawLine(p1, p2);
    }
}

QPointF GraphView::worldToScreen(const Point& point) const
{
    return worldToScreen(point.getX(), point.getY());
}

QPointF GraphView::worldToScreen(double x, double y) const
{
    return QPointF(x * m_scale + m_offset.x(), y * m_scale + m_offset.y());
}

QPointF GraphView::screenToWorld(const QPointF& screenPoint) const
{
    double x = (screenPoint.x() - m_offset.x()) / m_scale;
    double y = (screenPoint.y() - m_offset.y()) / m_scale;
    return QPointF(x, y);
}

Point GraphView::screenToWorldPoint(const QPointF& screenPoint) const
{
    double x = (screenPoint.x() - m_offset.x()) / m_scale;
    double y = (screenPoint.y() - m_offset.y()) / m_scale;
    return Point(x, y);
}

QRectF GraphView::getWorldBounds() const
{
    if (!m_graph || m_graph->getSize() == 0) {
        return QRectF();
    }
    
    double minX = m_graph->getPoint(0).getX();
    double maxX = minX;
    double minY = m_graph->getPoint(0).getY();
    double maxY = minY;
    
    for (size_t i = 1; i < m_graph->getSize(); ++i) {
        const auto& point = m_graph->getPoint(i);
        minX = std::min(minX, point.getX());
        maxX = std::max(maxX, point.getX());
        minY = std::min(minY, point.getY());
        maxY = std::max(maxY, point.getY());
    }
    
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

void GraphView::updateTransform()
{
    // Calcular viewport mundial baseado na transformação atual
    QPointF topLeft = screenToWorld(QPointF(0, 0));
    QPointF bottomRight = screenToWorld(QPointF(width(), height()));
    
    m_viewport = QRectF(topLeft.x(), topLeft.y(), 
                       bottomRight.x() - topLeft.x(), 
                       bottomRight.y() - topLeft.y());
}

QString GraphView::formatDistance(double distance) const
{
    return QString::number(distance, 'f', 1);
}

QColor GraphView::getPointColor(int index) const
{
    Q_UNUSED(index)
    return m_style.pointColor;
}

QColor GraphView::getRouteColor() const
{
    return m_style.routeColor;
}

void GraphView::initializeStyle()
{
    m_style.backgroundColor = QColor(248, 248, 248);
    m_style.gridColor = QColor(220, 220, 220);
    m_style.pointColor = QColor(52, 152, 219);
    m_style.pointSelectedColor = QColor(231, 76, 60);
    m_style.routeColor = QColor(46, 204, 113);
    m_style.routeAnimatedColor = QColor(230, 126, 34);
    m_style.textColor = QColor(52, 73, 94);
    
    m_style.pointPen = QPen(QColor(41, 128, 185), 2);
    m_style.routePen = QPen(m_style.routeColor, 3);
    m_style.gridPen = QPen(m_style.gridColor, 1);
    
    m_style.pointBrush = QBrush(m_style.pointColor);
    
    m_style.labelFont = QFont("Arial", 10);
    m_style.distanceFont = QFont("Arial", 8);
    
    m_style.pointRadius = 8;
    m_style.routeWidth = 3;
}