# ✅ MySQL على WSL - جاهز للاستخدام!

## ما تم إنجازه

1. ✅ **قاعدة البيانات**: `nctu_sis` تم إنشاؤها
2. ✅ **المستخدم**: `nctu_user` تم إنشاؤه
3. ✅ **الجداول**: جميع الجداول (15 جدول) تم إنشاؤها
4. ✅ **البيانات التجريبية**: تم إدخالها
5. ✅ **التطبيق**: محدث لاستخدام MySQL على WSL

## بيانات الاتصال

- **Host**: `localhost`
- **User**: `nctu_user`
- **Password**: `nctu_pass`
- **Database**: `nctu_sis`
- **Port**: `3306`

## التحقق من البيانات

```bash
# عرض الجداول
mysql -u nctu_user -pnctu_pass nctu_sis -e "SHOW TABLES;"

# عدد الطلاب
mysql -u nctu_user -pnctu_pass nctu_sis -e "SELECT COUNT(*) FROM students;"

# بيانات الطلاب
mysql -u nctu_user -pnctu_pass nctu_sis -e "SELECT student_code, name FROM students;"
```

## تشغيل التطبيق

```bash
cd /mnt/c/Users/yasee/.cursor/worktrees/UniversityApp/net/build
export DISPLAY=:0
./NCTU_SIS
```

### في نافذة إعدادات قاعدة البيانات:

- **Host**: `localhost`
- **Database**: `nctu_sis`
- **User**: `nctu_user`
- **Password**: `nctu_pass`
- **Port**: `3306`

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

## إدارة MySQL

```bash
# بدء MySQL
sudo service mysql start

# إيقاف MySQL
sudo service mysql stop

# حالة الخدمة
sudo service mysql status

# الدخول إلى MySQL
mysql -u nctu_user -pnctu_pass nctu_sis
```

## ملاحظات

- ✅ التطبيق محدث لاستخدام `localhost` و `nctu_user`
- ✅ جميع البيانات جاهزة للاستخدام
- ✅ يمكنك تشغيل التطبيق الآن!

---

**🎉 كل شيء جاهز! شغّل التطبيق واختبره!**

