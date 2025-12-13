# ✅ البناء نجح!

## المشروع تم بناؤه بنجاح في WSL

الملف التنفيذي موجود في:
```
/mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net/build/NCTU_SIS
```

## الخطوات التالية

### 1. إعداد MySQL على Windows (مطلوب قبل التشغيل)

**⚠️ مهم**: يجب إعداد MySQL على Windows للسماح بالاتصالات من WSL.

#### الطريقة السريعة (في PowerShell على Windows):

```powershell
cd C:\Users\yasee\.cursor\worktrees\UniversityApp\net
mysql -u root -pyaseentarek < setup_mysql_windows.sql
```

#### أو يدوياً في MySQL:

```sql
CREATE USER 'root'@'%' IDENTIFIED BY 'yaseentarek';
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;
```

### 2. إعداد قاعدة البيانات

بعد إعداد MySQL على Windows:

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
bash setup_database.sh
```

### 3. تشغيل التطبيق

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net/build
./NCTU_SIS
```

أو:

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net
./build/NCTU_SIS
```

## بيانات الاتصال

عند فتح التطبيق، استخدم:

- **Host**: `10.255.255.254` (Windows host IP) أو `localhost`
- **User**: `root`
- **Password**: `yaseentarek`
- **Database**: `nctu_sis`
- **Port**: `3306`

## بيانات تسجيل الدخول التجريبية

### طالب
- الرقم الجامعي: `UT241150`
- كلمة المرور: `student123`

### إداري
- البريد: `admin@nctu.edu.eg`
- كلمة المرور: `admin123`

### دكتور
- البريد: `ahmed.mohamed@nctu.edu.eg`
- كلمة المرور: `doctor123`

## ملاحظات

1. التطبيق سيكتشف تلقائياً IP Windows من `/etc/resolv.conf`
2. إذا فشل الاتصال، جرب `localhost` بدلاً من IP
3. تأكد من أن MySQL يعمل على Windows قبل التشغيل

## استكشاف الأخطاء

إذا واجهت مشاكل في الاتصال:
1. تحقق من أن MySQL يعمل: `Get-Service MySQL*` في PowerShell
2. اختبر الاتصال من WSL: `mysql -h 10.255.255.254 -u root -pyaseentarek -e "SELECT 1;"`
3. راجع `WSL_STATUS.md` و `QUICK_START_WSL.md` للمزيد من التفاصيل

---

**🎉 المشروع جاهز للتشغيل!**

