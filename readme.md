# Gerenciador de Arquivos no Disco

Este projeto implementa um gerenciador de arquivos em disco, simulando alocação e gerenciamento de memória para arquivos. Ele suporta diferentes estratégias de alocação e gerenciamento, como:

- **Alocação Contígua**
- **Alocação Encadeada**
- **Alocação Indexada**

## Compilação e Execução

### Requisitos
- Compilador C++ com suporte ao **C++11** ou superior.

### Compilação
Use o seguinte comando para compilar:

```bash
g++ simulator.cpp -o simulator -Wall
./simulator
```

## Funcionalidades

1. **Criação de Arquivos**: 
   - Cria arquivos usando diferentes estratégias de alocação.
   - Garante o uso eficiente dos blocos de memória.

2. **Exclusão de Arquivos**:
   - Remove arquivos e libera a memória alocada.

3. **Exibição de Memória**:
   - Visualiza o estado do disco e a ocupação dos blocos de memória.

4. **Visualização de Arquivos Específicos**:
   - Detalhes como nome, tamanho e localização dos blocos.

5. **Tratamento de Erros**:
   - Garante entradas válidas e fornece mensagens de erro úteis.

## Estrutura de Dados

### Estrutura `Block`
Representa um bloco no disco:
```cpp
struct Block {
    string color;          // Cor associada ao bloco do arquivo
    string file_name;      // Nome do arquivo
    int block_index;       // Índice do bloco
    int blocks_length;     // Número total de blocos usados pelo arquivo
    int start_block;       // Índice do bloco inicial
    Block *next;           // Ponteiro para o próximo bloco
    set<Block*> nexts_blocks; // Blocos relacionados (alocação indexada)
};
```


# Estrutura FileInfo
Armazena informações sobre o arquivo:
```cpp
struct FileInfo {
    string file_name_info;       // Nome do arquivo
    int logic_blocks_num_info;   // Quantidade de blocos lógicos necessários
};
```


## Cores Utilizadas
As cores são usadas para identificar arquivos visualmente. Exemplo de mapeamento:

- `\033[30m`: Preto
- `\033[31m`: Vermelho

Outras cores são configuradas no `initColors()`.

---

## Funções

### Inicialização
- **`initColors()`**: Inicializa o mapa de cores disponíveis.

### Manipulação de Arquivos
- **`contiguousFileCreate()`**: Cria arquivos usando alocação contígua.
- **`linkedFileCreate()`**: Cria arquivos usando alocação encadeada.
- **`indexedFileCreate()`**: Cria arquivos usando alocação indexada.
- **`contiguousFileDelete()`**: Remove arquivos alocados contiguamente.
- **`linkedFileDelete()`**: Remove arquivos alocados encadeadamente.
- **`recursiveDeleteLinkedFile()`**: Libera a memoria do arquivo deletado e cria novos blocos vazios aonde foi deletado.
- **`indexedFileDelete()`**: Remove arquivos alocados por índice.

### Descritor de arquivos
- **`findInFileDescriptor()`**: Verifica se o arquivo esta no descrito.
- **`deleteFileInDescriptor()`**: Exclui um arquivo do descritor.
- **`deleteFileInDescriptor()`**: Exclui um arquivo do descritor.


### Visualização

- **`showDiskMemory()`**: Exibe a memória do disco.
- **`showTableFileInDescriptor()`**: Chama a funcao para exibir informacoes do disco, toma decisao se e um arquivo especifico ou nao.
- **`showBlock()`**: Exibe informacoes dos arquivos.
- **`showSpecificFile()`**: Exibe detalhes de um arquivo específico.
- **`showLinkedBlocks()`**: Exibe os blocos recursivamente dos algoritimos CONTIGUO e ENCADEADO.
- **`showIndexTable()`**: Exibe os blocos guaradados na tabela de blocos de um arquivo no algoritimo INDEXADO.




### Auxiliares
- **`instructionAllocation()`**: Toma a decisao de qual funcao chamar para qual algoritimo (C || E || S).
- **`integerTreatment()`**: Valida entradas inteiras.
- **`findAvaliableColor()`**: Procura uma cor disponivel para um arquivo.
- **`findFreeBlocks()`**: Localiza blocos livres no disco.
- **`deepCopyVector()`**: Cria uma cópia do vetor de blocos para visualização.
- **`deleteDeepCopy()`**: Deleta a cópia criada do vetor de blocos para visualização.
- **`cinFileInfo()`**: Usado para dar as entradas nos arquivos que serao criados.
- **`verifyFileNameToDelete()`**: Verifica se o arquivo a ser deletado é um arquivo valido.


---

https://github.com/lucasbrun196/disk-memory-allocation-


