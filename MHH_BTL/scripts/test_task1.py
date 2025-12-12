import pytest
import os
import tempfile
from run import PetriNetParser  # Import class từ file run.py của bạn

# --- Helper Function: Tạo file PNML tạm thời ---
def create_temp_pnml(content):
    """Tạo một file .pnml tạm thời để test và trả về đường dẫn"""
    fd, path = tempfile.mkstemp(suffix=".pnml", text=True)
    with os.fdopen(fd, 'w') as f:
        f.write(content)
    return path

# --- TEST CASE 1: File chuẩn (Happy Path) ---
def test_parse_valid_simple_net():
    # Nội dung một file PNML đơn giản nhưng chuẩn
    valid_xml = """<?xml version="1.0" encoding="UTF-8"?>
    <pnml>
      <net id="net1">
        <page id="page0">
          <place id="p1">
            <name><text>Start</text></name>
            <initialMarking><text>1</text></initialMarking>
          </place>
          <place id="p2">
            <name><text>End</text></name>
          </place>
          <transition id="t1">
            <name><text>Process</text></name>
          </transition>
          <arc id="a1" source="p1" target="t1"/>
          <arc id="a2" source="t1" target="p2"/>
        </page>
      </net>
    </pnml>
    """
    path = create_temp_pnml(valid_xml)
    
    try:
        parser = PetriNetParser(path)
        result = parser.parse()
        
        # 1. Kiểm tra hàm parse trả về True
        assert result is True, "Parser phải trả về True với file hợp lệ"
        
        # 2. Kiểm tra số lượng phần tử
        assert len(parser.places) == 2, "Phải tìm thấy 2 places"
        assert len(parser.transitions) == 1, "Phải tìm thấy 1 transition"
        assert len(parser.arcs) == 2, "Phải tìm thấy 2 arcs"
        
        # 3. Kiểm tra thuộc tính (Token, Name)
        assert parser.places['p1']['tokens'] == 1, "Place p1 phải có 1 token"
        assert parser.places['p1']['name'] == "Start", "Tên place p1 phải là Start"
        assert parser.places['p2']['tokens'] == 0, "Place p2 mặc định phải là 0 token"
        
        # 4. Kiểm tra Cache (pre_set, post_set)
        # t1 nhận từ p1 và đẩy ra p2
        assert 'p1' in parser.pre_set['t1'], "t1 phải có input là p1"
        assert 'p2' in parser.post_set['t1'], "t1 phải có output là p2"

    finally:
        os.remove(path) # Dọn dẹp file tạm

# --- TEST CASE 2: File không tồn tại ---
def test_file_not_found():
    parser = PetriNetParser("duong_dan_khong_ton_tai.pnml")
    result = parser.parse()
    assert result is False, "Parser phải trả về False nếu file không tồn tại"

# --- TEST CASE 3: File lỗi cú pháp XML (Malformed) ---
def test_malformed_xml():
    # XML thiếu thẻ đóng </pnml>
    broken_xml = """<?xml version="1.0"?>
    <pnml>
        <net>
            <place id="p1"/>
    """ 
    path = create_temp_pnml(broken_xml)
    try:
        parser = PetriNetParser(path)
        result = parser.parse()
        assert result is False, "Parser phải trả về False nếu XML bị lỗi cú pháp"
    finally:
        os.remove(path)

# --- TEST CASE 4: File hợp lệ nhưng không có Place (Empty Net) ---
def test_empty_net():
    # File này đúng cú pháp nhưng không có place nào
    empty_xml = """<?xml version="1.0"?>
    <pnml><net><page></page></net></pnml>
    """
    path = create_temp_pnml(empty_xml)
    try:
        parser = PetriNetParser(path)
        result = parser.parse()
        # Dựa trên logic code của bạn: if not self.places: return False
        assert result is False, "Parser phải trả về False nếu không tìm thấy Place nào"
    finally:
        os.remove(path)

# --- TEST CASE 5: Kiểm tra tính nhất quán (Consistency) ---
def test_consistency_check():
    # Trường hợp 1: Có Arc -> Consistent
    valid_xml = """<pnml><net><page>
        <place id="p1"/><transition id="t1"/>
        <arc id="a1" source="p1" target="t1"/>
    </page></net></pnml>"""
    path1 = create_temp_pnml(valid_xml)
    
    # Trường hợp 2: Không có Arc -> Inconsistent (Rời rạc)
    disconnected_xml = """<pnml><net><page>
        <place id="p1"/><transition id="t1"/>
    </page></net></pnml>"""
    path2 = create_temp_pnml(disconnected_xml)

    try:
        # Test Case 1
        p1 = PetriNetParser(path1)
        p1.parse()
        assert p1.verify_consistency() is True, "Có arc thì consistency phải True"

        # Test Case 2
        p2 = PetriNetParser(path2)
        p2.parse()
        # Code hiện tại của bạn: return bool(self.arcs) -> False nếu không có arc
        assert p2.verify_consistency() is False, "Không có arc thì consistency phải False"
        
    finally:
        os.remove(path1)
        os.remove(path2)

# --- TEST CASE 6: Kiểm tra Arc ảo (Dangling Arc) ---
# Yêu cầu đề bài: "verify no missing nodes"
def test_dangling_arc():
    # Arc nối từ p1 (có thật) đến t_ao (không tồn tại)
    xml = """<pnml><net><page>
        <place id="p1"/>
        <arc id="a1" source="p1" target="t_ao"/>
    </page></net></pnml>"""
    path = create_temp_pnml(xml)
    
    try:
        parser = PetriNetParser(path)
        parser.parse()
        
        # Mặc dù parse() thành công (trả về True), nhưng verify_consistency nên kiểm tra logic này.
        # Lưu ý: Code hiện tại của bạn CHƯA xử lý việc check dangling nodes trong verify_consistency.
        # Test này dùng để nhắc nhở bạn cải thiện code (nếu cần).
        
        # Kiểm tra xem t_ao có lọt vào cache pre_set/post_set không?
        # Code của bạn: if arc['target'] in self.transitions: ...
        # Nên 't_ao' sẽ KHÔNG được thêm vào pre_set của ai cả.
        
        # Đây là kiểm tra logic code hiện tại của bạn có chạy đúng như đã viết không:
        assert 't_ao' not in parser.transitions
        # Arc vẫn được lưu trong self.arcs
        assert len(parser.arcs) == 1 
        
    finally:
        os.remove(path)