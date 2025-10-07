#include "RouteVisualizer.h"
#include <QPaintEvent>
#include <QDebug>
#include <cmath>
#include <algorithm>

RouteVisualizer::RouteVisualizer(QWidget *parent)
    : QWidget(parent)
    , m_animationProgress(0.0)
    , m_isAnimating(false)
    , m_animationDuration(2000)
    , m_easingType(QEasingCurve::InOutQuad)
    , m_showProgress(true)
    , m_scale(1.0)
    , m_offset(0, 0)
{
    setMinimumSize(200, 150);
    
    // Configurar animação
    m_animation = new QPropertyAnimation(this, "animationProgress");
    connect(m_animation, &QPropertyAnimation::finished, 
            this, &RouteVisualizer::onAnimationFinished);
    
    // Timer para atualizações suaves
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &RouteVisualizer::updateAnimation);
    
    initializeStyle();
}

RouteVisualizer::~RouteVisualizer() = default;

void RouteVisualizer::setRoute(std::shared_ptr<Route> route)
{
    m_route = route;
    
    if (m_route && m_route->getSize() > 0) {
        updateTransform();
    }
    
    resetAnimation();
    update();
}

void RouteVisualizer::startAnimation()
{
    if (!m_route || m_route->getSize() < 2) return;
    
    m_isAnimating = true;
    m_animation->setDuration(m_animationDuration);
    m_animation->setEasingCurve(m_easingType);
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);
    
    m_updateTimer->start(16); // ~60 FPS
    m_animation->start();
    
    emit animationStarted();
}

void RouteVisualizer::stopAnimation()
{
    m_animation->stop();
    m_updateTimer->stop();
    m_isAnimating = false;
}

void RouteVisualizer::resetAnimation()
{
    stopAnimation();
    setAnimationProgress(0.0);
}

void RouteVisualizer::setAnimationDuration(int durationMs)
{
    m_animationDuration = durationMs;
}

void RouteVisualizer::setAnimationEasing(QEasingCurve::Type easing)
{
    m_easingType = easing;
}

void RouteVisualizer::setShowProgress(bool show)
{
    m_showProgress = show;
    update();
}

bool RouteVisualizer::isAnimating() const
{
    return m_isAnimating;
}

double RouteVisualizer::animationProgress() const
{
    return m_animationProgress;
}

void RouteVisualizer::setAnimationProgress(double progress)
{
    m_animationProgress = std::max(0.0, std::min(1.0, progress));
    emit progressChanged(m_animationProgress);
    update();
}

void RouteVisualizer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fundo
    painter.fillRect(rect(), m_style.backgroundColor);
    
    if (m_route && m_route->getSize() > 0) {
        drawRoute(painter);
        
        if (m_showProgress) {
            drawRouteProgress(painter);
        }
        
        drawStatistics(painter);
    } else {
        // Desenhar mensagem quando não há rota
        painter.setPen(m_style.textColor);
        painter.setFont(m_style.statisticsFont);
        painter.drawText(rect(), Qt::AlignCenter, "Nenhuma rota para visualizar");
    }
}

void RouteVisualizer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    updateTransform();
}

void RouteVisualizer::updateAnimation()
{
    // Atualização adicional se necessário
    update();
}

void RouteVisualizer::onAnimationFinished()
{
    m_updateTimer->stop();
    m_isAnimating = false;
    emit animationFinished();
}

void RouteVisualizer::drawRoute(QPainter& painter)
{
    if (!m_route || m_route->getSize() < 2) return;
    
    const auto& points = m_route->getPoints();
    
    // Desenhar todos os pontos
    painter.setPen(QPen(m_style.textColor, 1));
    painter.setBrush(m_style.pointBrush);
    
    for (const auto& point : points) {
        QPointF screenPos = worldToScreen(point);
        painter.drawEllipse(screenPos, m_style.pointRadius, m_style.pointRadius);
    }
    
    // Desenhar rota base (cinza claro)
    painter.setPen(QPen(QColor(200, 200, 200), m_style.routeWidth));
    for (size_t i = 0; i < points.size() - 1; ++i) {
        QPointF p1 = worldToScreen(points[i]);
        QPointF p2 = worldToScreen(points[i + 1]);
        painter.drawLine(p1, p2);
    }
    
    // Fechar circuito
    if (points.size() > 2) {
        QPointF p1 = worldToScreen(points.back());
        QPointF p2 = worldToScreen(points.front());
        painter.drawLine(p1, p2);
    }
    
    // Desenhar progresso da animação
    double totalSegments = points.size(); // incluindo volta ao início
    double currentSegment = m_animationProgress * totalSegments;
    
    painter.setPen(QPen(m_style.animatedColor, m_style.animatedWidth));
    
    // Desenhar segmentos completos
    int completeSegments = int(currentSegment);
    for (int i = 0; i < completeSegments && i < int(points.size()) - 1; ++i) {
        QPointF p1 = worldToScreen(points[i]);
        QPointF p2 = worldToScreen(points[i + 1]);
        painter.drawLine(p1, p2);
    }
    
    // Desenhar segmento parcial
    double segmentProgress = currentSegment - completeSegments;
    if (segmentProgress > 0 && completeSegments < int(points.size())) {
        QPointF p1, p2;
        
        if (completeSegments < int(points.size()) - 1) {
            // Segmento normal
            p1 = worldToScreen(points[completeSegments]);
            p2 = worldToScreen(points[completeSegments + 1]);
        } else {
            // Último segmento (volta ao início)
            p1 = worldToScreen(points.back());
            p2 = worldToScreen(points.front());
        }
        
        QPointF partialEnd = p1 + (p2 - p1) * segmentProgress;
        painter.drawLine(p1, partialEnd);
    }
}

void RouteVisualizer::drawAnimatedSegment(QPainter& painter, int fromIndex, int toIndex, double progress)
{
    if (!m_route || fromIndex < 0 || toIndex < 0) return;
    
    const auto& points = m_route->getPoints();
    if (fromIndex >= int(points.size()) || toIndex >= int(points.size())) return;
    
    QPointF p1 = worldToScreen(points[fromIndex]);
    QPointF p2 = worldToScreen(points[toIndex]);
    QPointF current = p1 + (p2 - p1) * progress;
    
    painter.setPen(QPen(m_style.animatedColor, m_style.animatedWidth));
    painter.drawLine(p1, current);
}

void RouteVisualizer::drawRouteProgress(QPainter& painter)
{
    if (!m_route) return;
    
    // Barra de progresso no topo
    QRectF progressRect(10, 10, width() - 20, 8);
    
    // Fundo da barra
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.setBrush(QBrush(QColor(240, 240, 240)));
    painter.drawRect(progressRect);
    
    // Progresso
    QRectF filledRect = progressRect;
    filledRect.setWidth(progressRect.width() * m_animationProgress);
    
    painter.setBrush(QBrush(m_style.progressColor));
    painter.drawRect(filledRect);
    
    // Texto do progresso
    painter.setPen(m_style.textColor);
    painter.setFont(QFont("Arial", 9));
    QString progressText = QString("%1%").arg(int(m_animationProgress * 100));
    painter.drawText(progressRect.adjusted(0, -15, 0, 0), Qt::AlignCenter, progressText);
}

void RouteVisualizer::drawStatistics(QPainter& painter)
{
    if (!m_route) return;
    
    // Estatísticas no canto inferior
    painter.setPen(m_style.textColor);
    painter.setFont(m_style.statisticsFont);
    
    QStringList stats;
    stats << QString("Pontos: %1").arg(m_route->getSize());
    stats << QString("Distância: %1").arg(m_route->getTotalDistance(), 0, 'f', 2);
    
    if (m_isAnimating) {
        stats << QString("Progresso: %1%").arg(int(m_animationProgress * 100));
    }
    
    int y = height() - 15 * stats.size() - 10;
    for (const QString& stat : stats) {
        painter.drawText(10, y, stat);
        y += 15;
    }
}

QPointF RouteVisualizer::worldToScreen(const Point& point) const
{
    double x = point.getX() * m_scale + m_offset.x();
    double y = point.getY() * m_scale + m_offset.y();
    return QPointF(x, y);
}

QRectF RouteVisualizer::calculateBounds() const
{
    if (!m_route || m_route->getSize() == 0) {
        return QRectF(-50, -50, 100, 100);
    }
    
    const auto& points = m_route->getPoints();
    double minX = points[0].getX();
    double maxX = minX;
    double minY = points[0].getY();
    double maxY = minY;
    
    for (const auto& point : points) {
        minX = std::min(minX, point.getX());
        maxX = std::max(maxX, point.getX());
        minY = std::min(minY, point.getY());
        maxY = std::max(maxY, point.getY());
    }
    
    // Adicionar margem
    double margin = std::max(10.0, std::max(maxX - minX, maxY - minY) * 0.1);
    return QRectF(minX - margin, minY - margin, 
                  maxX - minX + 2 * margin, maxY - minY + 2 * margin);
}

void RouteVisualizer::updateTransform()
{
    QRectF bounds = calculateBounds();
    
    if (bounds.isEmpty()) {
        m_scale = 1.0;
        m_offset = QPointF(width() / 2.0, height() / 2.0);
        return;
    }
    
    // Calcular escala para caber na janela (deixar espaço para texto)
    double availableWidth = width() - 20;
    double availableHeight = height() - 80; // espaço para texto
    
    double scaleX = availableWidth / bounds.width();
    double scaleY = availableHeight / bounds.height();
    m_scale = std::min(scaleX, scaleY);
    
    // Centralizar
    m_offset = QPointF(width() / 2.0 - bounds.center().x() * m_scale,
                      height() / 2.0 - bounds.center().y() * m_scale + 20); // offset para texto
}

void RouteVisualizer::initializeStyle()
{
    m_style.backgroundColor = QColor(255, 255, 255);
    m_style.routeColor = QColor(180, 180, 180);
    m_style.animatedColor = QColor(46, 204, 113);
    m_style.progressColor = QColor(52, 152, 219);
    m_style.textColor = QColor(52, 73, 94);
    
    m_style.routePen = QPen(m_style.routeColor, 2);
    m_style.animatedPen = QPen(m_style.animatedColor, 3);
    m_style.pointBrush = QBrush(QColor(100, 100, 100));
    
    m_style.statisticsFont = QFont("Arial", 10);
    
    m_style.pointRadius = 4;
    m_style.routeWidth = 2;
    m_style.animatedWidth = 4;
}