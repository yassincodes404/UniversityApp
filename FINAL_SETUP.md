# ✅ الإعداد النهائي - MySQL على WSL

## حالة قاعدة البيانات

تم إنشاء قاعدة البيانات والجداول بنجاح! ✅

## بيانات الاتصال

### الخيار 1: استخدام root (بدون كلمة مرور - auth_socket)

- **Host**: `localhost`
- **User**: `root`
- **Password**: (اتركه فارغاً - يستخدم sudo authentication)
- **Database**: `nctu_sis`
- **Port**: `3306`

**ملاحظة**: في التطبيق، اترك حقل كلمة المرور فارغاً إذا كان MySQL يستخدم auth_socket.

### الخيار 2: إنشاء مستخدم بكلمة مرور

إذا أردت استخدام مستخدم بكلمة مرور، قم بتنفيذ:

```bash
sudo mysql << 'EOF'
CREATE USER 'nctu_user'@'localhost' IDENTIFIED BY 'nctu_pass';
GRANT ALL PRIVILEGES ON nctu_sis.* TO 'nctu_user'@'localhost';
FLUSH PRIVILEGES;
EOF
```

ثم استخدم:
- **User**: `nctu_user`
- **Password**: `nctu_pass`

## التحقق من البيانات

```bash
# عرض الجداول
sudo mysql nctu_sis -e "SHOW TABLES;"

# عدد الطلاب
sudo mysql nctu_sis -e "SELECT COUNT(*) FROM students;"

# بيانات الطلاب
sudo mysql nctu_sis -e "SELECT student_code, name FROM students;"
```

## تشغيل التطبيق

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net/build
export DISPLAY=:0
./NCTU_SIS
```

### في نافذة إعدادات قاعدة البيانات:

**إذا كان MySQL يستخدم auth_socket (الافتراضي في Ubuntu):**
- **Host**: `localhost`
- **Database**: `nctu_sis`
- **User**: `root`
- **Password**: (اتركه فارغاً)
- **Port**: `3306`

**إذا أنشأت مستخدم بكلمة مرور:**
- **Host**: `localhost`
- **Database**: `nctu_sis`
- **User**: `nctu_user`
- **Password**: `nctu_pass`
- **Port**: `3306`

## ملاحظة مهمة

إذا كان MySQL يستخدم `auth_socket` (الافتراضي)، قد تحتاج إلى تعديل إعدادات MySQL للسماح بالاتصال بكلمة مرور:

```bash
sudo mysql << 'EOF'
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'your_password';
FLUSH PRIVILEGES;
EOF
```

ثم استخدم كلمة المرور في التطبيق.

## بيانات تسجيل الدخول في التطبيق

### طالب
- الرقم الجامعي: `UT241150`
- كلمة المرور: `student123`

### إداري
- البريد: `admin@nctu.edu.eg`
- كلمة المرور: `admin123`

### دكتور
- البريد: `ahmed.mohamed@nctu.edu.eg`
- كلمة المرور: `doctor123`

---

**🎉 كل شيء جاهز! شغّل التطبيق الآن!**

