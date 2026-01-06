# Sistema de Estoque (C)

Projeto em C para controle simples de estoque via menu no terminal.
Permite cadastrar produtos, registrar entrada/saída, gerar relatório e salvar/carregar em CSV.

## Funcionalidades
- Cadastrar produto (código, nome, quantidade, preço)
- Entrada e saída de estoque (com validação)
- Relatório de itens abaixo do mínimo
- Salvar e carregar dados do arquivo `data/estoque.csv`

## Como compilar e executar

### Linux / macOS
```bash
gcc src/main.c -o app
./app
