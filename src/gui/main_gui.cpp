#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>
#include <QStyleFactory>
#include <QPalette>

#include "MainWindow.h"

/**
 * @brief Aplicação principal da GUI TSP Route Optimizer
 * 
 * Este arquivo implementa o ponto de entrada para a interface gráfica
 * do projeto TSP Route Optimizer, demonstrando a integração completa
 * de todos os conceitos POO em uma aplicação Qt6.
 * 
 * Conceitos POO demonstrados:
 * - Composição: QApplication contém widgets e janelas
 * - Herança: MainWindow herda de QMainWindow
 * - Polimorfismo: Eventos e slots do Qt
 * - Encapsulamento: Estado da aplicação e configurações
 */

void setupApplicationStyle(QApplication& app)
{
    // Configurar estilo visual moderno
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Paleta de cores personalizada
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    
    // Comentar a linha abaixo para usar tema claro
    // app.setPalette(darkPalette);
    
    // CSS personalizado para melhor aparência
    app.setStyleSheet(R"(
        QMainWindow {
            background-color: #f8f9fa;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 8px;
            margin-top: 1ex;
            padding-top: 10px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        
        QPushButton {
            background-color: #007bff;
            border: none;
            color: white;
            padding: 8px 16px;
            text-align: center;
            font-size: 14px;
            border-radius: 4px;
        }
        
        QPushButton:hover {
            background-color: #0056b3;
        }
        
        QPushButton:pressed {
            background-color: #004085;
        }
        
        QPushButton:disabled {
            background-color: #6c757d;
        }
        
        QComboBox {
            border: 1px solid #ced4da;
            border-radius: 4px;
            padding: 4px;
            background-color: white;
        }
        
        QSpinBox {
            border: 1px solid #ced4da;
            border-radius: 4px;
            padding: 4px;
            background-color: white;
        }
        
        QTextEdit {
            border: 1px solid #ced4da;
            border-radius: 4px;
            background-color: white;
        }
        
        QProgressBar {
            border: 1px solid #ced4da;
            border-radius: 4px;
            text-align: center;
        }
        
        QProgressBar::chunk {
            background-color: #28a745;
            border-radius: 4px;
        }
    )");
}

void showStartupMessage()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("TSP Route Optimizer - Etapa 3");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("<h3>Bem-vindo ao TSP Route Optimizer!</h3>");
    msgBox.setInformativeText(
        "<p><b>Projeto:</b> Programação Orientada a Objetos</p>"
        "<p><b>Autor:</b> Erick Batista da Silva</p>"
        "<p><b>Universidade:</b> UFPB - Centro de Informática</p>"
        "<br>"
        "<p><b>Como usar:</b></p>"
        "<ul>"
        "<li>Clique no mapa para adicionar pontos</li>"
        "<li>Selecione um algoritmo TSP</li>"
        "<li>Execute para ver a solução otimizada</li>"
        "<li>Use os controles para experimentar diferentes configurações</li>"
        "</ul>"
        "<br>"
        "<p><i>Esta GUI demonstra todos os conceitos POO em C++ com Qt6!</i></p>"
    );
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

int main(int argc, char *argv[])
{
    // Criar aplicação Qt
    QApplication app(argc, argv);
    
    // Configurar informações da aplicação
    QApplication::setApplicationName("TSP Route Optimizer");
    QApplication::setApplicationVersion("3.0.0");
    QApplication::setOrganizationName("UFPB - Centro de Informática");
    QApplication::setOrganizationDomain("ci.ufpb.br");
    QApplication::setApplicationDisplayName("TSP Route Optimizer - Etapa 3 GUI");
    
    // Configurar estilo visual
    setupApplicationStyle(app);
    
    try {
        // Criar e mostrar janela principal
        MainWindow mainWindow;
        mainWindow.show();
        
        // Mostrar mensagem de boas-vindas
        QTimer::singleShot(500, showStartupMessage);
        
        // Informações de debug
        qDebug() << "=== TSP Route Optimizer - Etapa 3 GUI ===";
        qDebug() << "Qt Version:" << QT_VERSION_STR;
        qDebug() << "Application started successfully";
        qDebug() << "========================================";
        
        // Executar loop principal da aplicação
        return app.exec();
        
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Erro Fatal", 
            QString("Erro na inicialização da aplicação:\n%1").arg(e.what()));
        return -1;
    } catch (...) {
        QMessageBox::critical(nullptr, "Erro Fatal", 
            "Erro desconhecido na inicialização da aplicação");
        return -1;
    }
}

/**
 * DEMONSTRAÇÃO DOS CONCEITOS POO NA GUI:
 * 
 * 1. CLASSES E OBJETOS:
 *    - MainWindow: Janela principal da aplicação
 *    - GraphView: Widget customizado para visualização
 *    - RouteVisualizer: Componente de animação
 *    - Point, Graph, Route: Classes de domínio TSP
 * 
 * 2. HERANÇA:
 *    - MainWindow herda de QMainWindow
 *    - GraphView herda de QWidget
 *    - RouteVisualizer herda de QWidget
 *    - TSPAlgorithm → NearestNeighborTSP, BruteForceTSP
 * 
 * 3. POLIMORFISMO:
 *    - Métodos virtuais dos algoritmos TSP
 *    - Override de paintEvent, mousePressEvent (Qt)
 *    - Slots e signals (sistema de eventos Qt)
 * 
 * 4. ENCAPSULAMENTO:
 *    - Dados privados em todas as classes
 *    - Interfaces públicas bem definidas
 *    - Getters/setters com validação
 * 
 * 5. COMPOSIÇÃO:
 *    - MainWindow contém GraphView
 *    - Graph contém Points
 *    - Route contém Points organizados
 * 
 * 6. STL E TEMPLATES:
 *    - std::vector para containers
 *    - std::unique_ptr para algoritmos
 *    - std::shared_ptr para objetos compartilhados
 * 
 * 7. SOBRECARGA DE OPERADORES:
 *    - Point: operator==, operator<, operator<<
 *    - Route: operator<, operator<<
 * 
 * 8. TRATAMENTO DE EXCEÇÕES:
 *    - TSPException personalizada
 *    - try/catch em operações críticas
 *    - Validação robusta de entrada
 * 
 * 9. SMART POINTERS:
 *    - unique_ptr para algoritmos e rotas
 *    - shared_ptr para objetos compartilhados
 *    - RAII automático
 * 
 * 10. PROGRAMAÇÃO ORIENTADA A EVENTOS:
 *     - Sistema signals/slots do Qt
 *     - Eventos de mouse e teclado
 *     - Timers para animação
 * 
 * 11. PADRÕES DE DESIGN:
 *     - Observer (signals/slots)
 *     - Strategy (algoritmos TSP)
 *     - Composite (widgets Qt)
 */