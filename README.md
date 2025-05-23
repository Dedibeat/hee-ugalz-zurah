# Улзий хээ: Алгоритмын псевдокод

## Функцүүдийн талаар

### Function: save(point)
- **Зорилго:** Одоогийн байрлалын координатыг хадгалах.
- **Үйлдэл:** 
  - Багцдаа (жишээ нь, жагсаалтад) нэмнэ:  
    **(Re(point), Im(point))**

---

### Function: move_forward(unit)
- **Зорилго:** Одоогийн чиглэл дагуу тухайн хэмжээтэй урагш хөдөлгөөн хийх.
- **Үйлдэл:**
  1. Одоогийн байрлалын координатыг хадгална (дараалалд нэмнэ).
  2. Шинэ байрлалд шинэчилнэ:  
     **new_position = current_position + (direction × unit)**

---

### Function: rotate(center, angle)
- **Зорилго:** Тодорхой төвийн эргэн тойронд байрлал, чиглэлийг тусдаа өнцөгөөр эргүүлэх.
- **Үйлдэл:**
  1. Одоогийн байрлалын координатыг хадгална.
  2. Векторыг тодорхойлно:  
     **vec = current_position − center**
  3. Эргэлтийг маш жижиг алхам болгож дүрслэх:
     - For i from 1 to 999:
       - Тооцоолох:  
         **incremental_angle = (angle × i) / 1000**
       - Тооцоолох дундад цэг:  
         **intermediate_point = center + vec × exp(i × incremental_angle)**
       - Хадгална intermediate_point‑ийг.
  4. Эцсийн эргэлтийн үйлдэл:
     - Шинэ байрлал:  
       **current_position = center + vec × exp(angle)**
     - Чиглэл шинэчилнэ:  
       **direction = direction × exp(angle)**

---

### Function: find_path()
- **Зорилго:** Хээ зурахын тулд бүгдийг дарааллаар гүйцэтгэж, замыг үүсгэх.
- **Үйлдэл:**
  1. Эхний байрлал болон чиглэл тохируулна:
     - Set current_position = (0, -50)
     - Set direction = (1, 0)
  2. Дараах ээлжлээр эргэлтүүдийг хэрэгжүүл:
     - **rotate(center = (0, 0), angle = –π)**
     - **rotate(center = (0, –25), angle = –π)**
     - **rotate(center = (0, 0), angle = –0.85 × π)**
     - **rotate(center = (–60, 130), angle = 0.6 × π)**
     - **rotate(center = (current_position + (50, 40)), angle = –0.45 × π)**
     - **rotate(center = (current_position + (–40, 30)), angle = 0.4 × π)**
  3. Тогтмол байдлаар эцсийн байрлалын утгыг гарган (хэмжээсийг хэвлэнэ).
  4. Эцсийн байрлалын цэгийг дараалалд нэмнэ.

---

### Function: display()
- **Зорилго:** Үүссэн замыг дэлгэц дээр дүрслэх.
- **Үйлдэл:**
  1. Цэсийг цэвэрлэнэ (үр дүнг цагаан өнгөөр дуудна).
  2. Ортографик 2D харагдах талбайг тохируулна (хоёр үгэнд –400 хүртэл, 400 хүртэл).
  3. Глобал эргүүлэх үйлдэл хийж (жишээ, -3° эргүүлэх) дүрсийг зөв байрлуулна.
  4. Дараалалд хадгалагдсан бүх координатуудын дагуу, дараалалын шугамыг (LINE_STRIP) зурна:
     - Үр дүнг хар өнгөөр дүрслэнэ.
  5. Дэлгэцийн буферуудыг солилцоно (үр дүнг ажигдуулах).

---

### Main Function
- **Зорилго:** Програмыг эхлүүлэх, тохиргоо хийх ба дүрслэх үйлдлийг дуудах.
- **Үйлдэл:**
  1. GLUT-ыг инициалчилна.
  2. Дүрслэх горимыг (double buffering болон RGB) тохируулна.
  3. Цонхын хэмжээг тохируулна (800 x 800).
  4. "УЛЗИЙ ХЭЭ" гэсэн гарчигтай цонх үүсгэнэ.
  5. Шугам зураг зурах чанарыг дээшлүүлэхээр (line smoothing, өндөр чанарын тохиргоо, шугамын өргөн) тохируулна.
  6. **find_path()** функцийг дуудах замаар зургийг үүсгэнэ.
  7. **display()** функцийг дүрслэх callback болгож бүртгэнэ.
  8. Дэлгэцийн өнгийг хар өнгөөр тохируулна.
  9. GLUT-ийн үндсэн үйлдлийн горимд орно.
  10. Програмыг дуусгасны дараа, тоглоомын утгыг буцаана (return value нь тогтвортой утга).

---
