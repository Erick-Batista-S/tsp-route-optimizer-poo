#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QProgressBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <memory>
#include "TSPClasses.h"

// Forward declarations das classes GUI
class GraphView;

/**
 * @brief Janela principal da aplicação TSP Route Optimizer
 * 
 * Esta classe implementa a interface gráfica principal que integra:
 * - Visualização interativa do grafo (GraphView)
 * - Controles para seleção de algoritmos
 * - Painel de métricas e resultados
 * - Menu e toolbar para operações de arquivo
 * 
 * Conceitos POO demonstrados:
 * - Composição: MainWindow contém GraphView e widgets Qt
 * - Encapsulamento: Interface pública bem definida
 * - Polimorfismo: Uso de diferentes algoritmos TSP
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots para ações do menu
    void newGraph();
    void openGraph();
    void saveGraph();
    void exportRoute();
    void showAbout();
    
    // Slots para controles de algoritmo
    void addRandomPoints();
    void clearGraph();
    void runSelectedAlgorithm();
    void onAlgorithmChanged();
    
    // Slots para resposta da visualização
    void onPointAdded(const Point& point);
    void onRouteUpdated(const Route& route);
    void onAlgorithmFinished(double distance, int timeMs);

private:
    // Métodos de inicialização
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void createControlPanel();
    void createMetricsPanel();
    void createResultsPanel();
    
    // Métodos auxiliares
    void updateMetrics();
    void resetMetrics();
    void updateAlgorithmInfo();
    std::unique_ptr<TSPAlgorithm> createSelectedAlgorithm();
    
    // Interface gráfica - Widgets principais
    GraphView* m_graphView;
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    QVBoxLayout* m_leftLayout;
    
    // Painel de controles
    QGroupBox* m_controlGroup;
    QComboBox* m_algorithmCombo;
    QSpinBox* m_pointCountSpin;
    QPushButton* m_addRandomBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_runBtn;
    QLabel* m_algorithmInfo;
    
    // Painel de métricas
    QGroupBox* m_metricsGroup;
    QLabel* m_pointCountLabel;
    QLabel* m_currentDistanceLabel;
    QLabel* m_bestDistanceLabel;
    QLabel* m_executionTimeLabel;
    QProgressBar* m_progressBar;
    
    // Painel de resultados
    QGroupBox* m_resultsGroup;
    QTextEdit* m_resultsText;
    
    // Menu e Actions
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_exportAction;
    QAction* m_exitAction;
    QAction* m_aboutAction;
    
    // Dados do modelo
    std::unique_ptr<Graph> m_graph;
    std::unique_ptr<Route> m_bestRoute;
    std::unique_ptr<TSPAlgorithm> m_currentAlgorithm;
    
    // Estado da aplicação
    bool m_isRunning;
    QTimer* m_timer;
    int m_executionTime;
};

#endif // MAINWINDOW_H