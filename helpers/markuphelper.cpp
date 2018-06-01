#include "markuphelper.h"

#include <stddef.h>
#include <stack>

#include <QChar>
#include <QString>

void appendCharAsHTML(QString& s, wchar_t c) {
  switch (c) {
    case L'<': s.append("&lt;"); break;
    case L'>': s.append("&gt;"); break;
    case L'\x22': s.append("&quot;"); break;
    case L'&': s.append("&amp;"); break;
    case L'\n': s.append("<br>"); break;
    default: s.append(c);
  }
}

QString MarkupHelper::markupToHTML(const QString& m) {
#define ADVANCE \
    do { ++i; if (i >= m.size()) return res; } while (false)
  QString res;
  std::stack<int> markup;
  int i = 0;
  for (; i < m.size(); ++i) {
    QChar c = m.at(i);
    int j = i;
    switch (c.unicode()) {
      case L'@': {
        ADVANCE;
        if (m.at(i) == QChar('(')) {
          markup.push(1);
          res.append("<hacm>");
        } else goto fallback;
        break;
      }
      case L')': {
        if (markup.top() == 1) {
          markup.pop();
          res.append("</hacm>");
          break;
        } else goto fallback;
      }
      case L'«': {
        markup.push(2);
        res.append("<span class=\"highlight\">");
        break;
      }
      case L'»': {
        if (markup.top() == 2) {
          markup.pop();
          res.append("</span>");
          break;
        } else goto fallback;
      }
      case L'\\': {
        ADVANCE;
        wchar_t d = m.at(i).unicode();
        appendCharAsHTML(res, d);
        break;
      }
      default:
      fallback:
        i = j;
        appendCharAsHTML(res, c.unicode());
    }
  }
  return res;
}
