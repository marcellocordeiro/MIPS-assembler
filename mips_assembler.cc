#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, string> type_r;
unordered_map<string, string> type_r_funct;
unordered_map<string, string> type_i;
unordered_map<string, string> type_j;

void initialize_unordered_maps ();

void print_type_r (string&, int);
void print_type_i (string&, int);
void print_type_j (string&, int);

void print_instruction (int, string&, string&);
inline string split_binary_string (string&, int, int);

ifstream input_file;
ofstream output_file;

int main (int argc, char *argv[]) {
  if (argc > 1) {
    input_file.open(argv[1]);
  } else {
    input_file.open("Assembly.txt");
  }

  if (!input_file.is_open()) {
    cout << "Input file not found" << endl;
    cout << "Press enter to exit" << endl;

    cin.get();
    return 1;
  }

  output_file.open("instrucoes.mif");

  initialize_unordered_maps();

  output_file << "WIDTH = 8;" << endl;
  output_file << "DEPTH = 256;" << endl << endl;
  output_file << "ADDRESS_RADIX = DEC;" << endl;
  output_file << "DATA_RADIX = BIN;" << endl << endl;
  output_file << "CONTENT" << endl << endl;
  output_file << "BEGIN" << endl << endl;

  int counter = 0;
  string opcode, instruction, binary_instruction;

  while (input_file >> opcode) {
    if (counter == 57) {
      cout << "Instruction limit exceeded" << endl;
      cout << "Press enter to exit" << endl;
  
      cin.get();
      return 1;
    }

    if (type_r.count(opcode) != 0) {
      print_type_r(opcode, counter++);
    } else if (type_i.count(opcode) != 0) {
      print_type_i(opcode, counter++);
    } else if (type_j.count(opcode) != 0) {
      print_type_j(opcode, counter++);
    } else {
      cout << "Invalid opcode" << endl;
      cout << "Press enter to exit" << endl;
  
      cin.get();
      return 1;
    }
  }

  input_file.close();

  instruction = "nop";
  binary_instruction = "00000000000000000000000000000000";

  while (counter < 57) {
    print_instruction(counter++, binary_instruction, instruction);
  }

  instruction = "excecao";
  binary_instruction = "00100000000111100000000000000001";
  print_instruction(counter++, binary_instruction, instruction);

  instruction = "excecao";
  binary_instruction = "00000000000000000000000000001101";
  print_instruction(counter++, binary_instruction, instruction);

  instruction = "nop";
  binary_instruction = "00000000000000000000000000000000";
  print_instruction(counter++, binary_instruction, instruction);

  instruction = "excecao";
  binary_instruction = "00111100000111100000000000000010";
  print_instruction(counter++, binary_instruction, instruction);

  instruction = "excecao";
  binary_instruction = "00000000000111101111010000000010";
  print_instruction(counter++, binary_instruction, instruction);

  instruction = "excecao";
  binary_instruction = "00000000000000000000000000001101";
  print_instruction(counter++, binary_instruction, instruction);

  instruction = "excecao";
  binary_instruction = "00000000000000001111000011100100";
  print_instruction(counter++, binary_instruction, instruction);

  output_file << "END;" << endl;

  output_file.close();

  return 0;
}

void print_instruction (int counter, string &binary_instruction, string &instruction) {
  output_file << setw(3) << setfill('0') << (counter*4) + 0 << ": " << split_binary_string(binary_instruction, 0,  8) << ';' << " --" << instruction << endl;
  output_file << setw(3) << setfill('0') << (counter*4) + 1 << ": " << split_binary_string(binary_instruction, 8,  8) << ';' << endl;
  output_file << setw(3) << setfill('0') << (counter*4) + 2 << ": " << split_binary_string(binary_instruction, 16, 8) << ';' << endl;
  output_file << setw(3) << setfill('0') << (counter*4) + 3 << ": " << split_binary_string(binary_instruction, 24, 8) << ';' << endl;

  output_file << endl << endl;
}

void print_type_r (string &opcode, int counter) {
  string instruction = opcode;
  int rd = 0, rs = 0, rt = 0, shamt = 0;

  if (opcode == "nop" || opcode == "break" || opcode == "rte") {
    // nothing
  } else if (opcode == "jr") {
    input_file >> rs;
    instruction += ' ' + to_string(rs);
  } else if (opcode == "sll" || opcode == "sra" || opcode == "srl") {
    input_file >> rd >> rt >> shamt;
    instruction += ' ' + to_string(rd) + ' ' + to_string(rt) + ' ' + to_string(shamt);
  } else if (opcode == "sllv" || opcode == "srav") {
    input_file >> rd >> rt >> rs;
    instruction += ' ' + to_string(rd) + ' ' + to_string(rt) + ' ' + to_string(rs);
  } else {
    input_file >> rd >> rs >> rt;
    instruction += ' ' + to_string(rd) + ' ' + to_string(rs) + ' ' + to_string(rt);
  }

  string binary_instruction = type_r[opcode]+ bitset<5>(rs).to_string() + bitset<5>(rt).to_string() + bitset<5>(rd).to_string() + bitset<5>(shamt).to_string() + type_r_funct[opcode];
  print_instruction(counter, binary_instruction, instruction);
}

void print_type_i (string &opcode, int counter) {
  string instruction = opcode;
  int rt = 0, rs = 0, immediate_offset_address = 0;

  if (opcode == "lui") {
    input_file >> rt >> immediate_offset_address;
    instruction += ' ' + to_string(rt) + ' ' + to_string(immediate_offset_address);
  } else if (opcode == "lbu" || opcode == "lhu" || opcode == "lw" || opcode == "sb" || opcode == "sh" || opcode == "sw") {
    input_file >> rt >> immediate_offset_address >> rs;
    instruction += ' ' + to_string(rt) + ' ' + to_string(immediate_offset_address) + ' ' + '(' + to_string(rs) + ')';
  } else if (opcode == "beq" || opcode == "bne") {
    input_file >> rs >> rt >> immediate_offset_address;
    instruction += ' ' + to_string(rs) + ' ' + to_string(rt) + ' ' + to_string(immediate_offset_address);
  } else {
    input_file >> rt >> rs >> immediate_offset_address;
    instruction += ' ' + to_string(rt) + ' ' + to_string(rs) + ' ' + to_string(immediate_offset_address);
  }

  string binary_instruction = type_i[opcode] + bitset<5>(rs).to_string() + bitset<5>(rt).to_string() + bitset<16>(immediate_offset_address).to_string();
  print_instruction(counter, binary_instruction, instruction);
}

void print_type_j (string &opcode, int counter) {
  string instruction = opcode;
  int offset = 0;

  input_file >> offset;
  instruction += ' ' + to_string(offset);

  string binary_instruction = type_j[opcode]+ bitset<26>(offset).to_string();
  print_instruction(counter, binary_instruction, instruction);
}

inline string split_binary_string (string &instruction, int position, int size) {
  return instruction.substr(position, size);
}

void initialize_unordered_maps () {
  type_r = {
    {"add"  , "000000"},
    {"addu" , "000000"},
    {"and"  , "000000"},
    {"sub"  , "000000"},
    {"xor"  , "000000"},
    {"slt"  , "000000"},
    {"subu" , "000000"},
    
    {"sll"  , "000000"},
    {"sra"  , "000000"},
    {"srl"  , "000000"},
    
    {"sllv" , "000000"},
    {"srav" , "000000"},
    
    {"jr"   , "000000"},
    
    {"break", "000000"},
    {"nop"  , "000000"},
    {"rte"  , "010000"}
  };
  
  type_r_funct = {
    {"add"  , "100000"},
    {"addu" , "100001"},
    {"and"  , "100100"},
    {"sub"  , "100010"},
    {"xor"  , "100110"},
    {"slt"  , "101010"},
    {"subu" , "100011"},
    
    {"sll"  , "000000"},
    {"sra"  , "000011"},
    {"srl"  , "000010"},
    
    {"sllv" , "000100"},
    {"srav" , "000111"},
    
    {"jr"   , "001000"},
    
    {"break", "001101"},
    {"nop"  , "000000"},
    {"rte"  , "010000"}
  };

  type_i = {
    {"addi" , "001000"},
    {"addiu", "001001"},
    {"andi" , "001100"},
    {"slti" , "001010"},
    {"sxori", "001110"},

    {"beq"  , "000100"},
    {"bne"  , "000101"},
    
    {"lbu"  , "100100"},
    {"lhu"  , "100101"},
    {"lw"   , "100011"},
    {"sb"   , "101000"},
    {"sh"   , "101001"},
    {"sw"   , "101011"},

    {"lui"  , "001111"}
  };

  type_j = {
    {"j"  , "000010"},
    {"jal", "000011"}
  };
}