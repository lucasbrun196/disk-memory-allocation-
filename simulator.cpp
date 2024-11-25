#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

#define END_COLOR "\033[0m"
#define EMPTY_BLOCK_COLOR "\033[1;47m"
#define DEFAULT_COLOR "\033[30m"
#define BOLD_TEXT "\033[1m"
#define WARNING_TEXT "\033[31m"

struct Block {
    string color, file_name;
    int block_index, blocks_length, start_block;
    Block *next;
    set<Block*> nexts_blocks; 
};

struct FileInfo {
    string file_name_info;
    int logic_blocks_num_info;
};

typedef vector<Block*> vb;

map<string, bool> used_colors_map;
vb file_descriptor;
bool isIndexed;
string global_specific_file;

void initColors(){
    used_colors_map["\033[30m"] = false;
    used_colors_map["\033[31m"] = false;
    used_colors_map["\033[32m"] = false;
    used_colors_map["\033[33m"] = false;
    used_colors_map["\033[34m"] = false;
    used_colors_map["\033[35m"] = false;
    used_colors_map["\033[36m"] = false;
    used_colors_map["\033[37m"] = false;
    used_colors_map["\033[90m"] = false;
    used_colors_map["\033[91m"] = false;
    used_colors_map["\033[92m"] = false;
    used_colors_map["\033[93m"] = false;
    used_colors_map["\033[94m"] = false;
    used_colors_map["\033[95m"] = false; 
    used_colors_map["\033[96m"] = false;
    used_colors_map["\033[97m"] = false;
}

bool findInFileDescriptor(string &file_name){
    for(Block *block: file_descriptor){
        if(block->file_name == file_name) return true;
    }
    return false;
}

void deleteFileInDescriptor(string &file_name){
    for(int i = 0; i < file_descriptor.size(); ++i){
        if(file_name == file_descriptor[i]->file_name){
            file_descriptor.erase(file_descriptor.begin() + i);
            break;
        }
    }
}

int integerTreatment(const string &msg){
    int integer_cin;
    while(true){
        cout << msg;
        cin >> integer_cin;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl << WARNING_TEXT << "A ENTRADA DEVE SER APENAS INTEIROS" << END_COLOR << endl << endl;
        }else break;
    }
    return integer_cin;
}

FileInfo cinFileInfo(vb &disk){
    while(true){
        string file_name_cin;
        cout << "DIGITE O NOME DO ARQUIVO: "; cin >> file_name_cin;
        cout << endl;
        if(!findInFileDescriptor(file_name_cin)){
            int logic_blocks_num_cin = integerTreatment("DIGITE O NUMERO DE BLOCOS LOGICOS QUE SERA OCUPADO: ");
            FileInfo fileInfo;
            fileInfo.file_name_info = file_name_cin;
            fileInfo.logic_blocks_num_info = logic_blocks_num_cin;
            return fileInfo;
        }else{
            cout << WARNING_TEXT << "JA EXISTE UM ARQUIVO COM ESTE NOME" << END_COLOR << endl << endl;
        }
    }
}

void showLinkedBlocks(Block *&block){
    if(block == nullptr){
        cout << endl;
        return;
    } 
    cout << block->block_index;
    if(block->next != nullptr) cout << "  -->  ";
    showLinkedBlocks(block->next);
}

void showIndexTable(Block *&block){
    int cont = 0;
    cout << "[  ";
    for(Block *b: block->nexts_blocks){
        cout << b->block_index;
        if(cont != block->nexts_blocks.size()-1) cout << " , ";
        cont++;
    }
    cout << "  ]\n";
}

void showBlock(Block *&block){
    cout << "FILE_NAME: " << block->file_name << endl;
    cout << "LENGTH: " << block->blocks_length << endl;
    cout << "COLOR_LEGEND: " << block->color << "[ COLOR ]" << END_COLOR << endl;
    if(isIndexed){
        cout << "INDEX_BLOCK "<< block->block_index << endl;
        cout << "INDEX_TABLE: "; showIndexTable(block);
    }else{
        cout << "START_BLOCK: " << block->start_block << endl;
        cout << "BLOCK_NUMBERS: "; showLinkedBlocks(block);
    }
}

void showTableFileInDescriptor(vb &disk){
    for(Block *block: file_descriptor){
        if(global_specific_file == "")showBlock(block);
        else{
            if(block->file_name == global_specific_file){
                showBlock(block);
                break;
            }
        }
        cout << endl;
    }
}


void showDiskMemory(vb &disk){
    cout << "|===============================================================================================|\n";
    for(int i = 0; i < disk.size(); ++i){
        if(i % 4 == 0) cout << endl;
        cout << " [ " << disk[i]->color << disk[i]->block_index << END_COLOR << " ] "; 
    }
    cout << endl << endl;
    showTableFileInDescriptor(disk); 
}

string findAvaliableColor(){
    for(auto color : used_colors_map){
        if(!color.second){
            used_colors_map[color.first] = true;
            return color.first;
        }
    }
    return DEFAULT_COLOR;
}

string verifyFileNameToDelete(){
    string file_name_cin;
    cout << "DIGITE O NOME DO ARQUIVO: "; cin >> file_name_cin;
    if(findInFileDescriptor(file_name_cin)) return file_name_cin;
    else{
        cout << endl << WARNING_TEXT << "O ARQUIVO DIGITADO NAO EXISTE" << END_COLOR << endl << endl;
        return "";
    } 
}

void contiguousFileCreate(vb &disk){
    FileInfo fileInfo = cinFileInfo(disk);
    int logic_blocks_num_cin, pre_index_allocator;
    string file_name_cin;
    bool find_space_flag;
    logic_blocks_num_cin = fileInfo.logic_blocks_num_info;
    file_name_cin = fileInfo.file_name_info;
    for(int i = 0; i < disk.size(); ++i){
        find_space_flag = false;
        if(disk[i]->next == nullptr && disk[i]->file_name == ""){
            pre_index_allocator = i;
            for(int j = i; j < logic_blocks_num_cin + (pre_index_allocator); ++j){
                if(j < disk.size() && disk[j]->next == nullptr && disk[j]->file_name == "") {
                    find_space_flag = true;
                    if(j == logic_blocks_num_cin + pre_index_allocator) break;
                }else{
                    find_space_flag = false;
                    break;
                }
            }
            if(find_space_flag) break;
        }
    }
    if((find_space_flag && ((pre_index_allocator + logic_blocks_num_cin) <= (disk.size())))){
        string avaliable_color = findAvaliableColor();
        for(int i = pre_index_allocator; i < logic_blocks_num_cin + pre_index_allocator; ++i){
            disk[i]->blocks_length = logic_blocks_num_cin;
            disk[i]->file_name = file_name_cin;
            disk[i]->start_block = pre_index_allocator;
            disk[i]->color = avaliable_color;
            if(i != logic_blocks_num_cin + pre_index_allocator-1) disk[i]->next = disk[i+1];
            else disk[i]->next = nullptr;
            if(i == pre_index_allocator) file_descriptor.push_back(disk[i]);
        } 
    }
    showDiskMemory(disk);
    if((find_space_flag == false || ((pre_index_allocator + logic_blocks_num_cin) > (disk.size())))) 
        cout << WARNING_TEXT << "VOCE NAO TEM ESPACO CONTIGUO DISPONIVEL NO DISCO POR FAVOR EXCLUA ALGUM ARQUIVO\n" << END_COLOR << endl;
}

void contiguousFileDelete(vb &disk){
    string file_name_cin = verifyFileNameToDelete();
    if(file_name_cin != ""){
        deleteFileInDescriptor(file_name_cin);
        cout << endl;
        for(int i = 0; i < disk.size(); ++i){
            if(disk[i]->file_name == file_name_cin){
                used_colors_map[disk[i]->color] = false;
                int length = disk[i]->blocks_length; 
                int cont_index = i;
                for(int j = i; j < i + length; ++j){
                    delete disk[j];
                    disk[j] = new Block;
                    disk[j]->block_index = cont_index;
                    disk[j]->next = nullptr;
                    disk[j]->color = EMPTY_BLOCK_COLOR;
                    disk[j]->file_name = "";
                    cont_index++;
                }
            }
        }
        cout << endl;
        showDiskMemory(disk);
    }
}

void deleteDeepCopy(vb &deep_copy){
    for(Block *block : deep_copy){
        block->next = nullptr;
        block->nexts_blocks.clear();
        delete block;
    } 
    deep_copy.clear();
}


vb deepCopyVector(vb &disk, string specific_file){
    vb deep_copy_disk;
    for(int i = 0; i < disk.size(); ++i){
        Block *copy_block = new Block;
        copy_block->block_index = i;
        copy_block->color = EMPTY_BLOCK_COLOR;
        copy_block->file_name = "";
        copy_block->next = nullptr;
        deep_copy_disk.push_back(copy_block);
    }
    int cont = 0;
    for(int i = 0; i < disk.size(); ++i){
        if(disk[i]->file_name == specific_file){
            deep_copy_disk[i]->color = disk[i]->color;
            deep_copy_disk[i]->file_name = disk[i]->file_name;
            deep_copy_disk[i]->start_block = disk[i]->start_block;
            deep_copy_disk[i]->blocks_length = disk[i]->blocks_length;
            if(isIndexed && disk[i]->nexts_blocks.size() > 0) for(Block *block : disk[i]->nexts_blocks) deep_copy_disk[i]->nexts_blocks.insert(deep_copy_disk[block->block_index]);
            else if(!isIndexed && cont != disk[i]->blocks_length-1)deep_copy_disk[i]->next = deep_copy_disk[disk[i]->next->block_index];
            cont++;
        }
    }
    return deep_copy_disk;
}

void recursiveDeleteLinkedFile(vb &disk, Block *block) {
    if (block == nullptr || block->file_name == "") return;
    int auxBlockIndex = block->block_index; 
    Block *nextBlock = block->next;         
    delete block;
    block = new Block;
    block->next = nullptr;
    block->color = EMPTY_BLOCK_COLOR;
    block->file_name = "";
    block->block_index = auxBlockIndex;
    disk[auxBlockIndex] = block;
    recursiveDeleteLinkedFile(disk, nextBlock);
}

void showSpecificFile(vb &disk){
    bool file_exist = false;
    string specific_file_name;
    string exit_aux;
    while(true){
        cout << "DESEJA VIZUALIZAR ALGUM ARQUIVO EM ESPECIFICO\nSE SIM DIGITE O NOME DELE, SE NAO APENAS DIGITE " << BOLD_TEXT << "NAO" << END_COLOR << " PARA CONTINUAR AS OPERACOES: ";
        cin >> specific_file_name;
        exit_aux = specific_file_name;
        transform(exit_aux.begin(), exit_aux.end(), exit_aux.begin(), ::toupper);
        if(exit_aux == "NAO") {
            global_specific_file = "";
            cout << endl;
            break;;
        }
        global_specific_file = specific_file_name;
        cout << endl;
        if(findInFileDescriptor(specific_file_name)){
            vb copy_disk = deepCopyVector(disk, specific_file_name);
            showDiskMemory(copy_disk);
            deleteDeepCopy(copy_disk);
            cout << endl;
        }else cout << WARNING_TEXT << "O ARQUIVO DIGITADO NAO EXISTE" << END_COLOR << endl << endl; 
    } 
}

vector<int> findFreeBlocks(vb &disk, int logic_blocks_num){
    vector<int> free_blocks_vec;
    for(int i = 0; i < disk.size(); ++i){
        if(disk[i]->next == nullptr && disk[i]->file_name == ""){
            free_blocks_vec.push_back(i);
            if(free_blocks_vec.size() == logic_blocks_num) break;
        }
    }
    return free_blocks_vec;
}

void linkedFileCreate(vb &disk){
    FileInfo fileInfo = cinFileInfo(disk);
    int logic_blocks_num_cin;
    string file_name_cin;
    logic_blocks_num_cin = fileInfo.logic_blocks_num_info;
    file_name_cin = fileInfo.file_name_info;
    vector<int> free_blocks_vec = findFreeBlocks(disk, logic_blocks_num_cin);
    if(free_blocks_vec.size() >= logic_blocks_num_cin){
        string avaliableColor = findAvaliableColor();
        int start_block_in_free = free_blocks_vec[0];
        for(int i = 0; i < free_blocks_vec.size(); ++i){
            disk[free_blocks_vec[i]]->file_name = file_name_cin;
            disk[free_blocks_vec[i]]->start_block = start_block_in_free;
            disk[free_blocks_vec[i]]->blocks_length = logic_blocks_num_cin;
            disk[free_blocks_vec[i]]->color = avaliableColor;
            if(i != free_blocks_vec.size()-1) disk[free_blocks_vec[i]]->next = disk[free_blocks_vec[i+1]];
            if(i == 0) file_descriptor.push_back(disk[free_blocks_vec[i]]);
        }
    }
    showDiskMemory(disk);
    if(free_blocks_vec.size() < logic_blocks_num_cin) cout << WARNING_TEXT << "VOCE NAO TEM ESPACO DISPONIVEL NO DISCO POR FAVOR EXCLUA ALGUM ARQUIVO\n" << END_COLOR << endl;
    if(file_descriptor.size() > 0) showSpecificFile(disk);
}


void linkedFileDelete(vb &disk){
    string file_name_cin = verifyFileNameToDelete();
    if(file_name_cin != ""){
        for(int i = 0; i < disk.size(); ++i){
            if(disk[i]->file_name == file_name_cin){
                deleteFileInDescriptor(disk[i]->file_name);
                used_colors_map[disk[i]->color] = false;
                recursiveDeleteLinkedFile(disk, disk[i]);
                break;
            }
        }
        showDiskMemory(disk);
        if(file_descriptor.size() > 0) showSpecificFile(disk);
    }
}

void indexedFileCreate(vb &disk){
    FileInfo fileInfo = cinFileInfo(disk);
    int logic_blocks_num_cin;
    string file_name_cin;
    logic_blocks_num_cin = fileInfo.logic_blocks_num_info;
    file_name_cin = fileInfo.file_name_info;
    vector<int> free_blocks_vec = findFreeBlocks(disk, logic_blocks_num_cin);
    if(free_blocks_vec.size() >= logic_blocks_num_cin){
        string avaliableColor = findAvaliableColor();
        int start_block_in_free = free_blocks_vec[0];
        for(int i = 0; i < free_blocks_vec.size(); ++i){
            disk[free_blocks_vec[i]]->file_name = file_name_cin;
            disk[free_blocks_vec[i]]->start_block = start_block_in_free;
            disk[free_blocks_vec[i]]->blocks_length = logic_blocks_num_cin;
            disk[free_blocks_vec[i]]->color = avaliableColor;
            if(i == 0) file_descriptor.push_back(disk[free_blocks_vec[i]]);
            else disk[start_block_in_free]->nexts_blocks.insert(disk[free_blocks_vec[i]]);
        }
    }
    showDiskMemory(disk);
    if(free_blocks_vec.size() < logic_blocks_num_cin) cout << WARNING_TEXT << "VOCE NAO TEM ESPACO DISPONIVEL NO DISCO POR FAVOR EXCLUA ALGUM ARQUIVO\n" << END_COLOR << endl;
    if(file_descriptor.size() > 0) showSpecificFile(disk);
}

void indexedFileDelete(vb &disk){
    string file_name_cin = verifyFileNameToDelete();
    if(file_name_cin != ""){
        for(Block *block : file_descriptor){
            if(block->file_name == file_name_cin){
                deleteFileInDescriptor(block->file_name);
                used_colors_map[block->color] = false;
                for(Block *blockInSet : block->nexts_blocks){
                    int aux_index = blockInSet->block_index;
                    delete blockInSet;
                    Block *newBlock = new Block;
                    newBlock->block_index = aux_index;
                    newBlock->color = EMPTY_BLOCK_COLOR;
                    newBlock->file_name = "";
                    newBlock->next = nullptr;
                    disk[aux_index] = newBlock;
                }
                int aux_index_block = block->block_index;
                delete block;
                Block *newBlockIndex = new Block;
                newBlockIndex->block_index = aux_index_block;
                newBlockIndex->color = EMPTY_BLOCK_COLOR;
                newBlockIndex->file_name = "";
                newBlockIndex->next = nullptr;
                disk[aux_index_block] = newBlockIndex;
                break;
            }
        }
        showDiskMemory(disk);
        if(file_descriptor.size() > 0) showSpecificFile(disk);
    }
}

void instructionAllocation(vb &disk, int locType){
    vb file_descriptor;
    char file_instruction;
    do{
        cout << "DESEJA CRIAR OU EXCLUIR UM ARQUIVO? " << BOLD_TEXT << "(C=criar  |  E=excluir  |  S=sair): " << END_COLOR;
        cin >> file_instruction;
        file_instruction = toupper(file_instruction);
        cout << endl;
        if(file_instruction != 'E' && file_instruction != 'C' && file_instruction != 'S'){
            cout << WARNING_TEXT << "OPCAO INVALIDA DIGITE APENAS (C || E || S)" << END_COLOR << endl << endl;
        }else{
            if(file_instruction == 'S') break;
            isIndexed = false;
            switch (locType){
            case 1:
                if(file_instruction == 'C') contiguousFileCreate(disk);
                else if(file_instruction == 'E') contiguousFileDelete(disk);
                break;
            case 2:
                if(file_instruction == 'C') linkedFileCreate(disk);
                else if(file_instruction == 'E') linkedFileDelete(disk);
                break;
            case 3:
                isIndexed = true;
                if(file_instruction == 'C') indexedFileCreate(disk);
                else if(file_instruction == 'E') indexedFileDelete(disk);
                break;
            default:
                break;
            }
        }
    } while (true);
}


int main(){
    int blocks_disk_size, allocation_type;
    vb disk;
    initColors();
    blocks_disk_size = integerTreatment("QUAL O TAMANHO DO DISCO EM BLOCOS? ");
    cout << endl;
    for(int i = 0; i < blocks_disk_size; ++i){
        Block *empty_block = new Block;
        empty_block->block_index = i;
        empty_block->color = EMPTY_BLOCK_COLOR;
        empty_block->next = nullptr;
        empty_block->file_name = "";
        disk.push_back(empty_block);
    }
    cout << "|===============================================================================================|\n";
    cout << "   ABAIXO SEU DISCO INICIADO COM NENHUM BLOCO ALOCADO: " << blocks_disk_size << " BLOCOS LIVRES\n";
    cout << "|===============================================================================================|\n";
    showDiskMemory(disk);
    while(true){
        allocation_type = integerTreatment("QUAL ESTRATEGIA DE ALOCAO VOCE QUER ESCOLHER:" + string(BOLD_TEXT) + "\n1: CONTIGUA\n2: ENCADEADA\n3: INDEXADA\n" + END_COLOR);
        cout << endl;
        if(allocation_type == 1 || allocation_type == 2 || allocation_type == 3) break;
        else cout << WARNING_TEXT "OPCAO INVALIDA" << END_COLOR << endl << endl;
    }
    instructionAllocation(disk, allocation_type);  
    return 0;
}