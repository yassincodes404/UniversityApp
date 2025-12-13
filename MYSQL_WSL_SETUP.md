# إعداد MySQL على WSL - مكتمل ✅

## ما تم إنجازه

1. ✅ **قاعدة البيانات**: `nctu_sis` تم إنشاؤها
2. ✅ **الجداول**: جميع الجداول (15 جدول) تم إنشاؤها
3. ✅ **البيانات التجريبية**: تم إدخالها بنجاح
4. ✅ **التطبيق**: محدث لاستخدام `localhost`

## بيانات الاتصال

- **Host**: `localhost`
- **User**: `root`
- **Password**: (كلمة مرور root الخاصة بك)
- **Database**: `nctu_sis`
- **Port**: `3306`

## إنشاء مستخدم مخصص (اختياري)

إذا أردت إنشاء مستخدم مخصص بدلاً من root:

```bash
mysql -u root -p <<EOF
CREATE USER 'nctu_user'@'localhost' IDENTIFIED BY 'nctu_pass';
GRANT ALL PRIVILEGES ON nctu_sis.* TO 'nctu_user'@'localhost';
FLUSH PRIVILEGES;
EOF
```

ثم استخدم:
- User: `nctu_user`
- Password: `nctu_pass`

## التحقق من البيانات

```bash
# عرض الجداول
mysql -u root -p nctu_sis -e "SHOW TABLES;"

# عدد الطلاب
mysql -u root -p nctu_sis -e "SELECT COUNT(*) FROM students;"

# عدد المقررات
mysql -u root -p nctu_sis -e "SELECT COUNT(*) FROM courses;"

# بيانات تسجيل الدخول
mysql -u root -p nctu_sis -e "SELECT student_code, name FROM students LIMIT 3;"
```

## تشغيل التطبيق

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net/build
export DISPLAY=:0
./NCTU_SIS
```

في نافذة إعدادات قاعدة البيانات:
- **Host**: `localhost`
- **Database**: `nctu_sis`
- **User**: `root`
- **Password**: (كلمة مرور root)
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

## إدارة MySQL

```bash
# بدء MySQL
sudo service mysql start

# إيقاف MySQL
sudo service mysql stop

# إعادة التشغيل
sudo service mysql restart

# حالة الخدمة
sudo service mysql status
```

## استكشاف الأخطاء

### خطأ: "Can't connect to MySQL server"

```bash
# تحقق من حالة MySQL
sudo service mysql status

# ابدأ MySQL
sudo service mysql start
```

### خطأ: "Access denied"

- تأكد من استخدام كلمة مرور root الصحيحة
- أو أنشئ مستخدم جديد (انظر أعلاه)

### خطأ: "Unknown database 'nctu_sis'"

```bash
# أنشئ قاعدة البيانات
mysql -u root -p -e "CREATE DATABASE nctu_sis CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;"
```

---

**✅ كل شيء جاهز! يمكنك الآن تشغيل التطبيق.**

