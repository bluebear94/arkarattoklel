#include "melpalthelper.h"

MelpaltHelper::MelpaltHelper() : ApplicationHelper()
{ }

const char* lantisShort[] = {
  "dia", "vio", "lis", "gil", "ful", "dyu", "mel",
  "ral", "zan", "pal", "mik", "fav", "ruj", "ser",
  "rav", "tan", "lin", "rez", "jil", "din", "ket",
  "len", "lax", "nen", "pin", "mat", "kun", "mir"
};
const char* myuxet[] = { "axet", "teems" };

QString MelpaltHelper::MelTimestamp::toString() const {
  QString res;
  res.append(QString::number(salt));
  res.append(" ");
  res.append(xelt == 14 ? "myuxet" : lantisShort[xelt - 1]);
  res.append(" ");
  res.append(xelt == 14 ? myuxet[sel - 1] : lantisShort[sel - 1]);
  res.append(" ");
  res.append(QString::number(miv));
  res.append(":");
  res.append(QString::number(fei));
  res.append(":");
  res.append(QString::number(jin));
  return res;
}

// 0 dia dia = 1988 Nov 30
constexpr time_t TIME_0_DIA_DIA = 596851200;

// 100 - 4 + 1 = (97) 366-day years
// 400 - 97 = (303) 365-day years
constexpr int DAYS_PER_400_YEARS = 97 * 366 + 303 * 365;

// 25 - 1 = (24) 366-day years
// 100 - 24 = (76) 365-day years
constexpr int DAYS_PER_100_YEARS = 24 * 366 + 76 * 365;

constexpr int DAYS_PER_4_YEARS = 366 + 3 * 365;

int eucd(int i, int n) {
  int q = i / n;
  return q - (n & (-(n * q > i)));
}

// Leap years are divisible by 4,
// and either not divisible by 100 or divisible by 400.
// The number of residual days are stored in `remaining`.
static int getYearsFromDay(int days, int& remaining) {
  int n400YearPeriods = eucd(days, DAYS_PER_400_YEARS);
  days -= n400YearPeriods * DAYS_PER_400_YEARS;
  if (days == 0) {
    remaining = 0;
    return 400 * n400YearPeriods;
  }
  // Year 0 is a leap year, but 100, 200 and 300 are not
  days -= 1;
  int n100YearPeriods = days / DAYS_PER_100_YEARS;
  days -= n100YearPeriods * DAYS_PER_100_YEARS;
  // Years 0, 4, etc. are leap years
  days += 1;
  int n4YearPeriods = days / DAYS_PER_4_YEARS;
  days -= n4YearPeriods * DAYS_PER_4_YEARS;
  int nYears =
    (days < 366) ? 0 :
    (days < 366 + 365) ? 1 :
    (days < 366 + 2 * 365) ? 2 : 3;
  remaining = days - (1 + nYears * 365);
  return nYears +
    4 * n4YearPeriods +
    100 * n100YearPeriods +
    400 * n400YearPeriods;
}

MelpaltHelper::MelTimestamp MelpaltHelper::create(const QDateTime& lipalt) {
  qint64 t = lipalt.toSecsSinceEpoch();
  int nDays = eucd(t - TIME_0_DIA_DIA, 86400);
  int secondsInDay = t - TIME_0_DIA_DIA - 86400 * nDays;
  int nRemainingDays;
  int nYears = getYearsFromDay(nDays, nRemainingDays);
  int month = nRemainingDays / 28;
  int day = nRemainingDays % 28;
  MelTimestamp m = {
    nYears,
    month + 1,
    day + 1,
    secondsInDay / 3600,
    (secondsInDay / 60) % 60,
    secondsInDay % 60
  };
  return m;
}

MelpaltHelper::MelTimestamp MelpaltHelper::create(const QString& l) {
  return MelpaltHelper::create(
    QDateTime::fromString(l, "yyyy-MM-dd'T'HH:mm:ss"));
}

QString MelpaltHelper::ltm(const QString& l) {
  return create(l).toString();
}

bool MelpaltHelper::isVowel(QChar c) {
  return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}
