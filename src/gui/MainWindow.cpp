#include "MainWindow.h"
#include "GraphView.h"
#include "RouteVisualizer.h"

#include <iostream>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_graphView(nullptr)
    , m_centralWidget(nullptr)
    , m_graph(std::make_unique<Graph>())
    , m_isRunning(false)
    , m_executionTime(0)
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    
    // Inicializar timer para atualizações
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateMetrics);
    
    // Configurar algoritmos disponíveis
    m_algorithmCombo->addItem("Nearest Neighbor");
    m_algorithmCombo->addItem("Brute Force");
    
    // Conectar sinais
    connect(m_algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onAlgorithmChanged);
    connect(m_addRandomBtn, &QPushButton::clicked, this, &MainWindow::addRandomPoints);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::clearGraph);
    connect(m_runBtn, &QPushButton::clicked, this, &MainWindow::runSelectedAlgorithm);
    
    // Conectar sinais do GraphView
    connect(m_graphView, &GraphView::pointAdded, this, &MainWindow::onPointAdded);
    
    // Inicializar GraphView com o Graph vazio
    auto graphPtr = std::shared_ptr<Graph>(m_graph.get(), [](Graph*){});
    m_graphView->setGraph(graphPtr);
    
    // Configuração inicial
    onAlgorithmChanged();
    resetMetrics();
    
    setWindowTitle("TSP Route Optimizer - Etapa 3 GUI");
    resize(1200, 800);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    // Layout principal
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    
    // GraphView (lado direito - maior)
    m_graphView = new GraphView;
    m_mainLayout->addWidget(m_graphView, 3);
    
    // Painel de controles (lado esquerdo)
    m_leftLayout = new QVBoxLayout;
    m_mainLayout->addLayout(m_leftLayout, 1);
    
    createControlPanel();
    createMetricsPanel();
    createResultsPanel();
    
    // Espaçador para empurrar tudo para cima
    m_leftLayout->addStretch();
}

void MainWindow::createControlPanel()
{
    m_controlGroup = new QGroupBox("Controles TSP");
    m_leftLayout->addWidget(m_controlGroup);
    
    auto layout = new QVBoxLayout(m_controlGroup);
    
    // Seleção de algoritmo
    layout->addWidget(new QLabel("Algoritmo:"));
    m_algorithmCombo = new QComboBox;
    layout->addWidget(m_algorithmCombo);
    
    m_algorithmInfo = new QLabel;
    m_algorithmInfo->setWordWrap(true);
    m_algorithmInfo->setStyleSheet("QLabel { color: #666; font-style: italic; }");
    layout->addWidget(m_algorithmInfo);
    
    layout->addSpacing(10);
    
    // Controles de pontos
    auto pointsLayout = new QHBoxLayout;
    pointsLayout->addWidget(new QLabel("Pontos:"));
    m_pointCountSpin = new QSpinBox;
    m_pointCountSpin->setRange(3, 20);
    m_pointCountSpin->setValue(5);
    pointsLayout->addWidget(m_pointCountSpin);
    layout->addLayout(pointsLayout);
    
    m_addRandomBtn = new QPushButton("Adicionar Pontos Aleatórios");
    layout->addWidget(m_addRandomBtn);
    
    m_clearBtn = new QPushButton("Limpar Grafo");
    layout->addWidget(m_clearBtn);
    
    layout->addSpacing(10);
    
    // Botão de execução
    m_runBtn = new QPushButton("Executar Algoritmo");
    m_runBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    layout->addWidget(m_runBtn);
}

void MainWindow::createMetricsPanel()
{
    m_metricsGroup = new QGroupBox("Métricas");
    m_leftLayout->addWidget(m_metricsGroup);
    
    auto layout = new QGridLayout(m_metricsGroup);
    
    // Labels de métricas
    layout->addWidget(new QLabel("Pontos:"), 0, 0);
    m_pointCountLabel = new QLabel("0");
    layout->addWidget(m_pointCountLabel, 0, 1);
    
    layout->addWidget(new QLabel("Distância Atual:"), 1, 0);
    m_currentDistanceLabel = new QLabel("-");
    layout->addWidget(m_currentDistanceLabel, 1, 1);
    
    layout->addWidget(new QLabel("Melhor Distância:"), 2, 0);
    m_bestDistanceLabel = new QLabel("-");
    layout->addWidget(m_bestDistanceLabel, 2, 1);
    
    layout->addWidget(new QLabel("Tempo:"), 3, 0);
    m_executionTimeLabel = new QLabel("-");
    layout->addWidget(m_executionTimeLabel, 3, 1);
    
    // Barra de progresso
    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar, 4, 0, 1, 2);
}

void MainWindow::createResultsPanel()
{
    m_resultsGroup = new QGroupBox("Resultados");
    m_leftLayout->addWidget(m_resultsGroup);
    
    auto layout = new QVBoxLayout(m_resultsGroup);
    
    m_resultsText = new QTextEdit;
    m_resultsText->setMaximumHeight(150);
    m_resultsText->setReadOnly(true);
    layout->addWidget(m_resultsText);
}

void MainWindow::setupMenuBar()
{
    // Menu Arquivo
    auto fileMenu = menuBar()->addMenu("&Arquivo");
    
    m_newAction = new QAction("&Novo", this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newGraph);
    fileMenu->addAction(m_newAction);
    
    m_openAction = new QAction("&Abrir", this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openGraph);
    fileMenu->addAction(m_openAction);
    
    m_saveAction = new QAction("&Salvar", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveGraph);
    fileMenu->addAction(m_saveAction);
    
    fileMenu->addSeparator();
    
    m_exportAction = new QAction("&Exportar Rota", this);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportRoute);
    fileMenu->addAction(m_exportAction);
    
    fileMenu->addSeparator();
    
    m_exitAction = new QAction("Sai&r", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(m_exitAction);
    
    // Menu Ajuda
    auto helpMenu = menuBar()->addMenu("&Ajuda");
    
    m_aboutAction = new QAction("&Sobre", this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Pronto - Clique no mapa para adicionar pontos");
}

// Slots para ações do menu
void MainWindow::newGraph()
{
    clearGraph();
    statusBar()->showMessage("Novo grafo criado");
}

void MainWindow::openGraph()
{
    // Implementação básica - pode ser expandida para carregar de arquivo
    QMessageBox::information(this, "Abrir", "Funcionalidade de abrir arquivo será implementada em versão futura");
}

void MainWindow::saveGraph()
{
    // Implementação básica - pode ser expandida para salvar em arquivo
    QMessageBox::information(this, "Salvar", "Funcionalidade de salvar arquivo será implementada em versão futura");
}

void MainWindow::exportRoute()
{
    if (!m_bestRoute || m_bestRoute->getSize() == 0) {
        QMessageBox::warning(this, "Exportar", "Nenhuma rota para exportar");
        return;
    }
    QMessageBox::information(this, "Exportar", "Funcionalidade de exportar será implementada em versão futura");
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "Sobre TSP Route Optimizer",
        "<h3>TSP Route Optimizer - Etapa 3</h3>"
        "<p><b>Projeto:</b> Programação Orientada a Objetos</p>"
        "<p><b>Autor:</b> Erick Batista da Silva</p>"
        "<p><b>Universidade:</b> UFPB - Centro de Informática</p>"
        "<p><b>Matrícula:</b> 20240010967</p>"
        "<br>"
        "<p>Interface gráfica para otimização de rotas usando diferentes algoritmos TSP.</p>"
        "<p>Demonstra todos os conceitos de POO em C++ com Qt6.</p>");
}

// Slots para controles
void MainWindow::addRandomPoints()
{
    int count = m_pointCountSpin->value();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-100.0, 100.0);
    
    try {
        for (int i = 0; i < count; ++i) {
            Point point(dis(gen), dis(gen), "P" + std::to_string(m_graph->getSize() + 1));
            m_graph->addPoint(point);
        }
        
        // Atualizar visualização
        auto graphPtr = std::shared_ptr<Graph>(m_graph.get(), [](Graph*){});
        m_graphView->setGraph(graphPtr);
        
        updateMetrics();
        statusBar()->showMessage(QString("Adicionados %1 pontos aleatórios").arg(count));
        
    } catch (const TSPException& e) {
        QMessageBox::warning(this, "Erro", QString("Erro ao adicionar pontos: %1").arg(e.what()));
    }
}

void MainWindow::clearGraph()
{
    m_graph->clear();
    m_bestRoute.reset();
    
    // Atualizar GraphView com o graph limpo
    auto graphPtr = std::shared_ptr<Graph>(m_graph.get(), [](Graph*){});
    m_graphView->setGraph(graphPtr);
    
    resetMetrics();
    m_resultsText->clear();
    
    statusBar()->showMessage("Grafo limpo");
}

void MainWindow::runSelectedAlgorithm()
{
    if (m_graph->getSize() < 3) {
        QMessageBox::warning(this, "Aviso", "É necessário pelo menos 3 pontos para executar o TSP");
        return;
    }
    
    m_isRunning = true;
    m_runBtn->setEnabled(false);
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Modo indeterminado
    
    // Criar algoritmo selecionado
    auto algorithm = createSelectedAlgorithm();
    
    // Executar algoritmo e medir tempo
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        Route result = algorithm->solve(*m_graph);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Atualizar resultado
        m_bestRoute = std::make_unique<Route>(result);
        m_executionTime = duration.count();
        
        // Atualizar visualização
        auto routePtr = std::shared_ptr<Route>(m_bestRoute.get(), [](Route*){});
        m_graphView->setRoute(routePtr);
        
        // Atualizar interface
        updateMetrics();
        
        // Adicionar resultado ao painel
        QString resultText = QString("%1:\nDistância: %2\nTempo: %3ms\nPontos: %4\n\n")
            .arg(QString::fromStdString(algorithm->getName()))
            .arg(m_bestRoute->getTotalDistance(), 0, 'f', 2)
            .arg(m_executionTime)
            .arg(m_bestRoute->getSize());
        
        m_resultsText->append(resultText);
        
        statusBar()->showMessage(QString("Algoritmo executado: %1 (distância: %2)")
            .arg(QString::fromStdString(algorithm->getName()))
            .arg(m_bestRoute->getTotalDistance(), 0, 'f', 2));
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erro", QString("Erro na execução: %1").arg(e.what()));
    }
    
    m_isRunning = false;
    m_runBtn->setEnabled(true);
    m_progressBar->setVisible(false);
}

void MainWindow::onAlgorithmChanged()
{
    updateAlgorithmInfo();
}

void MainWindow::updateAlgorithmInfo()
{
    auto algorithm = createSelectedAlgorithm();
    if (algorithm) {
        m_algorithmInfo->setText(QString::fromStdString(algorithm->getDescription()));
    }
}

std::unique_ptr<TSPAlgorithm> MainWindow::createSelectedAlgorithm()
{
    int index = m_algorithmCombo->currentIndex();
    switch (index) {
        case 0: return std::make_unique<NearestNeighborTSP>();
        case 1: return std::make_unique<BruteForceTSP>();
        default: return std::make_unique<NearestNeighborTSP>();
    }
}

// Slots para resposta da visualização
void MainWindow::onPointAdded(const Point& point)
{
    try {
        m_graph->addPoint(point);
        
        // Atualizar visualização com o graph atualizado
        auto graphPtr = std::shared_ptr<Graph>(m_graph.get(), [](Graph*){});
        m_graphView->setGraph(graphPtr);
        
        updateMetrics();
        statusBar()->showMessage(QString("Ponto adicionado: (%1, %2)")
            .arg(point.getX(), 0, 'f', 1)
            .arg(point.getY(), 0, 'f', 1));
    } catch (const TSPException& e) {
        QMessageBox::warning(this, "Erro", QString("Erro ao adicionar ponto: %1").arg(e.what()));
    }
}

void MainWindow::onRouteUpdated(const Route& route)
{
    Q_UNUSED(route)
    updateMetrics();
}

void MainWindow::onAlgorithmFinished(double distance, int timeMs)
{
    statusBar()->showMessage(QString("Algoritmo concluído - Distância: %1, Tempo: %2ms")
        .arg(distance, 0, 'f', 2).arg(timeMs));
}

void MainWindow::updateMetrics()
{
    m_pointCountLabel->setText(QString::number(m_graph->getSize()));
    
    if (m_bestRoute && m_bestRoute->getSize() > 0) {
        m_currentDistanceLabel->setText(QString::number(m_bestRoute->getTotalDistance(), 'f', 2));
        m_bestDistanceLabel->setText(QString::number(m_bestRoute->getTotalDistance(), 'f', 2));
        m_executionTimeLabel->setText(QString("%1ms").arg(m_executionTime));
    }
}

void MainWindow::resetMetrics()
{
    m_pointCountLabel->setText("0");
    m_currentDistanceLabel->setText("-");
    m_bestDistanceLabel->setText("-");
    m_executionTimeLabel->setText("-");
    m_executionTime = 0;
}