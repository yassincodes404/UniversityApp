-- Seed Data for NCTU SIS
-- This file contains initial test data

-- Insert Departments
INSERT INTO departments (id, name, description) VALUES
(1, 'Computer Science', 'Computer Science and Information Technology Department'),
(2, 'Engineering', 'Engineering Department'),
(3, 'Business', 'Business Administration Department');

-- Insert Programs
INSERT INTO programs (id, name, department_id, years) VALUES
(1, 'Computer Science', 1, 4),
(2, 'Information Technology', 1, 4),
(3, 'Software Engineering', 1, 4),
(4, 'Mechanical Engineering', 2, 4),
(5, 'Business Administration', 3, 4);

-- Insert Labs
INSERT INTO labs (id, lab_name, capacity) VALUES
(1, 'Lab 101', 30),
(2, 'Lab 102', 25),
(3, 'Lab 201', 35),
(4, 'Lab 301', 30);

-- Insert Admin (password: admin123, hashed with salt)
-- Hash: SHA256("salt123" + "admin123") = e3449f1bbcbc22aa87271112484e18b4b7bcd6d1bd2e4dab54817a3ded30d185
INSERT OR REPLACE INTO admins (id, name, email, password_hash, salt) VALUES
(1, 'System Administrator', 'admin@nctu.edu.eg', 'e3449f1bbcbc22aa87271112484e18b4b7bcd6d1bd2e4dab54817a3ded30d185', 'salt123');

-- Insert Doctors (password: doctor123)
-- Hash: SHA256("salt123" + "doctor123") = 7c8c7fd2792d4b0b1e3e2dad1465de8933557e029fd9154d4a4835d4200d776c
INSERT OR REPLACE INTO doctors (id, name, email, phone, password_hash, salt, department_id) VALUES
(1, 'Dr. Ahmed Mohamed', 'ahmed.mohamed@nctu.edu.eg', '01001234567', '7c8c7fd2792d4b0b1e3e2dad1465de8933557e029fd9154d4a4835d4200d776c', 'salt123', 1),
(2, 'Dr. Sara Ali', 'sara.ali@nctu.edu.eg', '01009876543', '7c8c7fd2792d4b0b1e3e2dad1465de8933557e029fd9154d4a4835d4200d776c', 'salt123', 1),
(3, 'Dr. Mohamed Hassan', 'mohamed.hassan@nctu.edu.eg', '01005556677', '7c8c7fd2792d4b0b1e3e2dad1465de8933557e029fd9154d4a4835d4200d776c', 'salt123', 2);

-- Insert Students
-- Student login:
--   - Username: numeric student_code (e.g., 20241150)
--   - Password: national_id for that student
-- Hashes below are SHA256("salt123" + national_id)
INSERT OR REPLACE INTO students (id, student_code, name, email, phone, password_hash, salt, program_id, department_id, national_id, birth_date, gender, seat_number, status, level) VALUES
(1, '20241150', 'Yassin Mohamed', 'yassin.mohamed@student.nctu.edu.eg', '01001112233', '79d2a78f37a2359a6090ee34f04e061c0049eaac80ab11135a23f3fd25479f2f', 'salt123', 1, 1, '12345678901234', '2000-01-15', 'M', 'S001', 'active', 2),
(2, '20241151', 'Mariam Ali', 'mariam.ali@student.nctu.edu.eg', '01002223344', 'c2bbd888faac430b418317fcc14da40ab7f3f3b8e79fda3194d5b89b1e4d899c', 'salt123', 1, 1, '23456789012345', '2001-03-20', 'F', 'S002', 'active', 2),
(3, '20241152', 'Omar Hassan', 'omar.hassan@student.nctu.edu.eg', '01003334455', 'd1166c99c1a35d4a7aed51f463c14a1de4475d8cda447424cb19001494156c81', 'salt123', 2, 1, '34567890123456', '2000-07-10', 'M', 'S003', 'active', 3);

-- Insert Courses
INSERT INTO courses (id, code, name, credit_hours, program_id, year, semester, course_type, description) VALUES
(1, 'CS101', 'Introduction to Programming', 3, 1, 1, 'Fall', 'Mixed', 'Basic programming concepts and algorithms'),
(2, 'CS102', 'Data Structures', 3, 1, 2, 'Fall', 'Theory', 'Data structures and algorithms'),
(3, 'CS201', 'Database Systems', 3, 1, 2, 'Spring', 'Mixed', 'Database design and SQL'),
(4, 'CS301', 'Software Engineering', 3, 1, 3, 'Fall', 'Theory', 'Software development methodologies'),
(5, 'IT101', 'Web Development', 3, 2, 1, 'Fall', 'Mixed', 'HTML, CSS, JavaScript basics'),
(6, 'IT201', 'Network Security', 3, 2, 2, 'Spring', 'Theory', 'Network security principles');

-- Insert Sections
INSERT INTO sections (id, section_type, course_id, doctor_id, lab_id, day, time_slot, location) VALUES
(1, 'Lecture', 1, 1, NULL, 'Sunday', '09:00-10:30', 'Hall A'),
(2, 'Lab', 1, 1, 1, 'Sunday', '11:00-13:00', 'Lab 101'),
(3, 'Lecture', 2, 1, NULL, 'Monday', '09:00-10:30', 'Hall B'),
(4, 'Section', 2, 1, NULL, 'Monday', '11:00-12:30', 'Room 201'),
(5, 'Lecture', 3, 2, NULL, 'Tuesday', '09:00-10:30', 'Hall A'),
(6, 'Lab', 3, 2, 2, 'Tuesday', '11:00-13:00', 'Lab 102'),
(7, 'Lecture', 4, 2, NULL, 'Wednesday', '09:00-10:30', 'Hall C'),
(8, 'Lecture', 5, 3, NULL, 'Sunday', '14:00-15:30', 'Hall B'),
(9, 'Lab', 5, 3, 3, 'Sunday', '16:00-18:00', 'Lab 201');

-- Insert Enrollments (current semester: Fall 2024, semester=1)
INSERT INTO enrollments (id, student_id, course_id, semester, year, status, attempt_number) VALUES
(1, 1, 1, 1, 2024, 'registered', 1),
(2, 1, 2, 1, 2024, 'registered', 1),
(3, 1, 3, 1, 2024, 'registered', 1),
(4, 2, 1, 1, 2024, 'registered', 1),
(5, 2, 2, 1, 2024, 'registered', 1),
(6, 3, 5, 1, 2024, 'registered', 1);

-- Insert Student Sections
INSERT INTO student_sections (student_id, section_id) VALUES
(1, 1), (1, 2), -- Yassin in CS101 Lecture and Lab
(1, 3), (1, 4), -- Yassin in CS102 Lecture and Section
(1, 5), (1, 6), -- Yassin in CS201 Lecture and Lab
(2, 1), (2, 2), -- Mariam in CS101 Lecture and Lab
(2, 3), (2, 4), -- Mariam in CS102 Lecture and Section
(3, 8), (3, 9); -- Omar in IT101 Lecture and Lab

-- Insert Sample Grades
INSERT INTO grades (id, enrollment_id, student_id, course_id, midterm1, midterm2, practical, assignments, final_exam, total, letter_grade, result_status, is_published) VALUES
(1, 1, 1, 1, 18, 20, 15, 8, 35, 96.0, 'A', 'pass', 1),
(2, 2, 1, 2, 17, 19, NULL, 9, 38, 83.0, 'B', 'pass', 1),
(3, 4, 2, 1, 16, 18, 14, 7, 32, 87.0, 'B+', 'pass', 1);

-- Insert Sample Attendance
INSERT INTO attendance (student_id, section_id, attendance_date, status) VALUES
(1, 1, '2024-09-01', 'present'),
(1, 1, '2024-09-08', 'present'),
(1, 1, '2024-09-15', 'late'),
(1, 2, '2024-09-01', 'present'),
(1, 2, '2024-09-08', 'present'),
(2, 1, '2024-09-01', 'present'),
(2, 1, '2024-09-08', 'absent'),
(2, 1, '2024-09-15', 'present');

-- Insert Sample Payments
INSERT INTO payments (id, student_id, amount, semester, payment_date, method, status) VALUES
(1, 1, 5000.00, 1, '2024-08-15', 'Online', 'Paid'),
(2, 2, 5000.00, 1, '2024-08-20', 'Cash', 'Paid'),
(3, 3, 5000.00, 1, NULL, 'Online', 'Pending');

-- Insert Sample News
INSERT INTO news (id, title, content, news_date, created_by) VALUES
(1, 'Welcome to Fall 2024 Semester', 'Welcome all students to the Fall 2024 semester. Classes begin on September 1st.', '2024-08-25', 1),
(2, 'Registration Deadline Extended', 'Course registration deadline has been extended to September 5th.', '2024-08-28', 1),
(3, 'Midterm Exam Schedule', 'Midterm exams will be held from October 15-20. Check your schedule.', '2024-09-10', 1);

-- Insert Sample Exams
INSERT INTO exams (id, course_id, exam_type, exam_date, year, semester, duration_minutes, venue) VALUES
(1, 1, 'Midterm', '2024-10-15', 2024, 1, 90, 'Hall A'),
(2, 1, 'Final', '2024-12-20', 2024, 1, 120, 'Hall A'),
(3, 2, 'Midterm', '2024-10-16', 2024, 1, 90, 'Hall B');

