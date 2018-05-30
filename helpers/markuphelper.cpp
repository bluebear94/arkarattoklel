#include "markuphelper.h"

#include <stack>

#include <QChar>
#include <QString>

QString MarkupHelper::markupToHTML(const QString& m) {
#define ADVANCE \
    do { ++i; if (i >= m.size()) return res; } while (false)
  QString res;
  int i = 0;
  for (; i < m.size(); ++i) {
    QChar c = m.at(i);
    int j = i;
    switch (c.unicode()) {
      case L'@': {
        ADVANCE;
        if (m.at(i) == QChar('(')) {
          ADVANCE;
          res.append("<hacm>");
          while (i < m.size() && m.at(i) != ')') {
            res.append(m.at(i));
            ++i;
          }
          res.append("</hacm>");
        } else goto fallback;
        break;
      }
      case L'<': res.append("&lt;"); break;
      case L'>': res.append("&gt;"); break;
      case L'\x22': res.append("&quot;"); break;
      case L'&': res.append("&amp;"); break;
      case L'\n': res.append("<br>"); break;
      default:
      fallback:
        i = j;
        res.append(c);
    }
  }
  return res;
}
