# 🎮 TSP Route Optimizer - Guia de Demonstração GUI

## 🚀 Como Executar a Aplicação

### 1. Compilar e Executar
```bash
cd tsp-route-optimizer-poo/build
make tsp_gui
./bin/tsp_gui
```

### 2. Interface Principal

A aplicação abrirá com:
- **Mapa interativo** (lado direito) para visualização
- **Painel de controles** (lado esquerdo) com opções
- **Menu superior** com funcionalidades adicionais

## 🎯 Tutorial de Uso

### Passo 1: Adicionar Pontos
- **Método 1**: Clique diretamente no mapa para adicionar cidades
- **Método 2**: Use "Adicionar Pontos Aleatórios" com número desejado

### Passo 2: Selecionar Algoritmo
- **Nearest Neighbor**: Rápido, solução aproximada
- **Brute Force**: Lento, solução ótima (máx 8-10 pontos)

### Passo 3: Executar TSP
- Clique "Executar Algoritmo"
- Observe a animação da solução sendo construída
- Veja métricas em tempo real

### Passo 4: Experimentar
- Limpe o grafo e teste diferentes configurações
- Compare algoritmos com os mesmos pontos
- Explore os controles de visualização

## 🎭 Conceitos POO Demonstrados

### Durante o Uso da GUI:

1. **Cliques no Mapa** → Demonstram **Herança** (eventos Qt)
2. **Seleção de Algoritmos** → Demonstram **Polimorfismo** (Strategy pattern)
3. **Animação de Rotas** → Demonstra **Composição** (widgets + dados)
4. **Tratamento de Erros** → Demonstra **Exceções** (validações)
5. **Métricas Automáticas** → Demonstram **Encapsulamento** (cálculos internos)

### Observações Técnicas:

- **Smart Pointers**: Algoritmos gerenciados automaticamente
- **STL**: Containers para pontos e rotas
- **Sobrecarga**: Operadores para comparação de rotas
- **RAII**: Limpeza automática de recursos

## 📊 Exemplos de Teste

### Teste 1: Comparação de Algoritmos
1. Adicione 5 pontos aleatórios
2. Execute Nearest Neighbor → anote resultado
3. Execute Brute Force → compare resultado
4. Observe diferenças de tempo e qualidade

### Teste 2: Casos Extremos
1. Teste com 3 pontos (mínimo para TSP)
2. Teste com 8+ pontos (Brute Force fica lento)
3. Teste pontos em linha reta vs. dispersos

### Teste 3: Interface Responsiva
1. Redimensione a janela
2. Teste zoom e pan no mapa
3. Verifique atualização de métricas

## 🎥 Gravação do Vídeo (Para Entrega)

### Roteiro Sugerido:

1. **Abertura** (30s)
   - Mostrar aplicação inicializando
   - Explicar objetivo do projeto

2. **Demonstração CLI** (60s)
   - Executar ./bin/tsp_optimizer
   - Mostrar saída com conceitos POO

3. **Demonstração GUI** (90s)
   - Abrir ./bin/tsp_gui
   - Adicionar pontos interativamente
   - Executar algoritmos diferentes
   - Mostrar animação e resultados

4. **Conceitos POO** (60s)
   - Narrar conceitos sendo demonstrados
   - Destacar herança, polimorfismo, etc.

5. **Conclusão** (30s)
   - Resumir implementação completa
   - Mencionar todos os requisitos atendidos

### Dicas de Gravação:
- Use OBS Studio ou software similar
- Grave em 1080p com áudio claro
- Mantenha duração entre 4-5 minutos
- Faça script para não gaguejar
- Teste gravação antes da versão final

## 📧 Entrega Final

### Email para Professor:
```
Para: bidu@ci.ufpb.br
Assunto: [POO-251-E003-3] 20240010967

Professor,

Segue a entrega da Etapa 3 (FINAL) do projeto TSP Route Optimizer.

Repositório: https://github.com/Erick-Batista-S/tsp-route-optimizer-poo
Tag da Entrega: v3-gui-final
Executáveis: 
- build/bin/tsp_optimizer (CLI - Etapa 2)
- build/bin/tsp_gui (GUI - Etapa 3)

Interface gráfica completa implementada com Qt6, demonstrando todos os conceitos POO conforme especificação.

Vídeo de demonstração: [ANEXAR LINK/ARQUIVO]

Atenciosamente,
Erick Batista da Silva
Matrícula: 20240010967
```

## ✅ Checklist Final

- [ ] GUI compilando e executando
- [ ] Todos os conceitos POO demonstrados
- [ ] README atualizado
- [ ] Vídeo gravado (4-5 min)
- [ ] Commit e tag v3-gui-final
- [ ] Push para GitHub
- [ ] Email enviado para professor

**🎉 Projeto TSP Route Optimizer - Etapa 3 Completa! 🎉**