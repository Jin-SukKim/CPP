/*
int main() {
  MyExcel::CSVTable table(5, 5);
  std::ofstream out("test.csv");

  table.reg_cell(new Cell("Hello~", 0, 0, &table), 0, 0);
  table.reg_cell(new Cell("C++", 0, 1, &table), 0, 1);

  table.reg_cell(new Cell("Programming", 1, 1, &table), 1, 1);
  out << table;

  MyExcel::HtmlTable table2(5, 5);
  std::ofstream out2("test.html");

  table2.reg_cell(new Cell("Hello~", 0, 0, &table), 0, 0);
  table2.reg_cell(new Cell("C++", 0, 1, &table), 0, 1);
  table2.reg_cell(new Cell("Programming", 1, 1, &table), 1, 1);
  out2 << table2;
}
*/