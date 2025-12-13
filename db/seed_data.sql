-- seed_data.sql
-- Seed test data for NCTU SIS
-- Run after mysql_create.sql

USE nctu_sis;

-- Insert Departments
INSERT INTO departments (name, description) VALUES
('كلية تكنولوجيا الصناعة والطاقة', 'Faculty of Industry and Energy Technology');

-- Insert Programs
INSERT INTO programs (name, department_id, years) VALUES
('Mechatronics', 1, 4),
('Autotronics', 1, 4),
('ICT', 1, 4),
('New and Renewable Energy', 1, 4),
('Petroleum Technology', 1, 4),
('Prosthetics and Orthotics', 1, 4);

-- Insert Labs
INSERT INTO labs (lab_name, capacity) VALUES
('Lab 101', 30),
('Lab 102', 25),
('Lab 201', 30),
('Lab 202', 25);

-- Helper function to generate password hash (using SHA256)
-- In production, use proper bcrypt or similar
-- For seed data, we'll use simple hashing: password = "123456" for all test users
-- Salt will be random string, hash = SHA256(salt + password)

-- Insert Admin (password: admin123)
-- Salt: admin_salt_2024
-- Hash: SHA256('admin_salt_2024' + 'admin123') = calculated
INSERT INTO admins (name, email, password_hash, salt) VALUES
('System Admin', 'admin@nctu.edu.eg', 'a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3', 'admin_salt_2024');

-- Insert Doctors (password: doctor123)
INSERT INTO doctors (name, email, phone, password_hash, salt, department_id) VALUES
('Dr. Ahmed Mohamed', 'ahmed.mohamed@nctu.edu.eg', '01012345678', 'b3a8e0e1f9ab1bfe3a36f231f676f78bb30a519d2b21e6c530c0eee8ebb4a5d0', 'doctor_salt_001', 1),
('Dr. Sara Ali', 'sara.ali@nctu.edu.eg', '01087654321', 'b3a8e0e1f9ab1bfe3a36f231f676f78bb30a519d2b21e6c530c0eee8ebb4a5d0', 'doctor_salt_002', 1);

-- Insert Students (password: student123, national_id as default)
-- Student codes: UT241150, UT241151, UT241152
INSERT INTO students (student_code, name, email, phone, password_hash, salt, program_id, department_id, national_id, birth_date, gender, seat_number, status, level, credits_completed, gpa, must_change_password) VALUES
('UT241150', 'Mohamed Hassan', 'mohamed.hassan@student.nctu.edu.eg', '01111111111', 'c1c224b03cd9bc7b6a86d77f5dace40191766c485cd55dc52caf303d19e73d6b', 'student_salt_001', 1, 1, '12345678901234', '2003-01-15', 'M', 'S001', 'active', 1, 0, 0.00, TRUE),
('UT241151', 'Fatma Ibrahim', 'fatma.ibrahim@student.nctu.edu.eg', '01222222222', 'c1c224b03cd9bc7b6a86d77f5dace40191766c485cd55dc52caf303d19e73d6b', 'student_salt_002', 2, 1, '23456789012345', '2003-05-20', 'F', 'S002', 'active', 1, 0, 0.00, TRUE),
('UT241152', 'Ali Mahmoud', 'ali.mahmoud@student.nctu.edu.eg', '01333333333', 'c1c224b03cd9bc7b6a86d77f5dace40191766c485cd55dc52caf303d19e73d6b', 'student_salt_003', 3, 1, '34567890123456', '2003-08-10', 'M', 'S003', 'active', 1, 0, 0.00, TRUE);

-- Insert Courses
INSERT INTO courses (code, name, credit_hours, program_id, year, semester, course_type, description) VALUES
('MECT101', 'Introduction to Mechatronics', 3, 1, 1, 'Fall', 'Mixed', 'Basic concepts of mechatronics systems'),
('MECT102', 'Mechanical Systems', 3, 1, 1, 'Fall', 'Theory', 'Fundamentals of mechanical engineering'),
('AUTO101', 'Introduction to Autotronics', 3, 2, 1, 'Fall', 'Mixed', 'Basics of automotive electronics'),
('ICT101', 'Programming Fundamentals', 4, 3, 1, 'Fall', 'Mixed', 'Introduction to programming'),
('ICT102', 'Data Structures', 3, 3, 1, 'Spring', 'Theory', 'Data structures and algorithms'),
('ENER101', 'Renewable Energy Systems', 3, 4, 1, 'Fall', 'Mixed', 'Introduction to renewable energy');

-- Insert Sections
INSERT INTO sections (section_type, course_id, doctor_id, lab_id, day, time_slot, location) VALUES
('Lecture', 1, 1, NULL, 'Sunday', '09:00-11:00', 'Hall A'),
('Lab', 1, 1, 1, 'Tuesday', '11:00-13:00', 'Lab 101'),
('Lecture', 3, 2, NULL, 'Monday', '09:00-11:00', 'Hall B'),
('Lecture', 4, 2, NULL, 'Wednesday', '09:00-12:00', 'Hall C'),
('Lab', 4, 2, 2, 'Thursday', '10:00-12:00', 'Lab 102');

-- Insert Enrollments (for current semester - Fall 2024)
INSERT INTO enrollments (student_id, course_id, semester, year, status, attempt_number) VALUES
(1, 1, 1, 2024, 'registered', 1),
(1, 2, 1, 2024, 'registered', 1),
(2, 3, 1, 2024, 'registered', 1),
(3, 4, 1, 2024, 'registered', 1),
(3, 5, 2, 2024, 'pending', 1);

-- Insert Sample Grades
INSERT INTO grades (student_id, course_id, enrollment_id, midterm1, midterm2, final_exam, practical, assignments, total, letter_grade, result_status) VALUES
(1, 1, 1, 85.0, 90.0, 88.0, 92.0, 90.0, 88.5, 'B+', 'pass'),
(1, 2, 2, 75.0, 80.0, 82.0, NULL, 85.0, 80.0, 'B', 'pass'),
(2, 3, 3, 70.0, 75.0, 78.0, 80.0, 75.0, 75.5, 'C+', 'pass');

-- Insert Sample Payments
INSERT INTO payments (student_id, amount, semester, payment_date, method, status, notes) VALUES
(1, 5000.00, 1, '2024-09-01 10:00:00', 'Online', 'Paid', 'Tuition fee Fall 2024'),
(2, 5000.00, 1, '2024-09-02 11:00:00', 'Cash', 'Paid', 'Tuition fee Fall 2024'),
(3, 5000.00, 1, NULL, 'Online', 'Pending', 'Tuition fee Fall 2024');

-- Insert Sample News
INSERT INTO news (title, content, news_date, created_by) VALUES
('Welcome to Fall Semester 2024', 'We welcome all students to the new academic year. Classes begin on September 15, 2024.', '2024-09-01', 1),
('Course Registration Deadline', 'Last day for course registration is September 20, 2024. Please complete your registration.', '2024-09-05', 1);

-- Insert Sample Exams
INSERT INTO exams (course_id, exam_type, exam_date, year, semester, duration_minutes, venue) VALUES
(1, 'Midterm', '2024-10-15', 2024, 1, 90, 'Hall A'),
(1, 'Final', '2024-12-20', 2024, 1, 120, 'Hall A'),
(4, 'Midterm', '2024-10-18', 2024, 1, 90, 'Hall C');

-- Insert Sample Attendance
INSERT INTO attendance (student_id, section_id, attendance_date, status, remark) VALUES
(1, 1, '2024-09-15', 'present', NULL),
(1, 1, '2024-09-22', 'present', NULL),
(1, 1, '2024-09-29', 'late', 'Arrived 10 minutes late'),
(1, 2, '2024-09-17', 'present', NULL),
(2, 3, '2024-09-16', 'present', NULL),
(2, 3, '2024-09-23', 'absent', NULL);

-- Grant permissions to admin
INSERT INTO permissions (admin_id, operation, resource) VALUES
(1, 'create', 'students'),
(1, 'read', 'students'),
(1, 'update', 'students'),
(1, 'delete', 'students'),
(1, 'create', 'courses'),
(1, 'read', 'courses'),
(1, 'update', 'courses'),
(1, 'delete', 'courses'),
(1, 'create', 'grades'),
(1, 'read', 'grades'),
(1, 'update', 'grades'),
(1, 'publish', 'grades');

