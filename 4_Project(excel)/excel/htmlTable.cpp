namespace MyExcel
{
class HtmlTable : public virtual Table {
 public:
  HtmlTable(int row, int col);

  string print_table();
};

HtmlTable::HtmlTable(int row, int col) : Table(row, col) {}

string HtmlTable::print_table() {
  string s = "<table border='1' cellpadding='10'>";
  for (int i = 0; i < max_row_size; i++) {
    s += "<tr>";
    for (int j = 0; j < max_col_size; j++) {
      s += "<td>";
      if (data_table[i][j]) s += data_table[i][j]->stringify();
      s += "</td>";
    }
    s += "</tr>";
  }
  s += "</table>";
  return s;
}
}