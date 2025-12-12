import pytest
import os
import tempfile
from run import PetriNetParser, ReachabilityGraph, DeadlockDetector, HAS_PULP

# --- Helper: Tạo file PNML ---
def create_temp_pnml(content):
    fd, path = tempfile.mkstemp(suffix=".pnml", text=True)
    with os.fdopen(fd, 'w') as f:
        f.write(content)
    return path

# --- Helper: Chạy Detector và bắt Output ---
def run_deadlock_check(xml_content, capsys):
    if not HAS_PULP:
        pytest.skip("Thư viện 'pulp' chưa được cài đặt. Bỏ qua Task 4.")

    path = create_temp_pnml(xml_content)
    try:
        # 1. Parse
        parser = PetriNetParser(path)
        parser.parse()
        
        # 2. Tìm tập Reachable (Dùng Task 2)
        solver_bfs = ReachabilityGraph(parser)
        all_states_list = solver_bfs.compute_reachability() # Trả về list[dict]
        
        # --- BƯỚC QUAN TRỌNG: CHUYỂN ĐỔI DỮ LIỆU ---
        # Chuyển list[dict] thành set[tuple] để DeadlockDetector hiểu được
        reachable_set = set()
        for state_dict in all_states_list:
            # Chuyển dict thành tuple đã sắp xếp (hashable)
            state_tuple = tuple(sorted(state_dict.items()))
            reachable_set.add(state_tuple)
        # ---------------------------------------------
        
        # 3. Chạy Deadlock Detector
        detector = DeadlockDetector(parser, reachable_set)
        detector.run()
        
        # 4. Bắt kết quả in ra màn hình
        captured = capsys.readouterr()
        return captured.out
        
    finally:
        os.remove(path)

# --- TC 4.1: Vòng lặp vĩnh cửu (No Deadlock) ---
# P1 -> T1 -> P2 -> T2 -> P1. Luôn có transition bắn được.
def test_cycle_no_deadlock(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/><transition id="t2"/>
      <arc source="p1" target="t1"/><arc source="t1" target="p2"/>
      <arc source="p2" target="t2"/><arc source="t2" target="p1"/>
    </page></net></pnml>"""
    
    output = run_deadlock_check(xml, capsys)
    assert "DEADLOCK FOUND" not in output

# --- TC 4.2: Ngõ cụt (Simple Deadlock) ---
# P1 -> T1 -> P2. (P2 không có lối ra -> Deadlock)
def test_simple_deadlock(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
    </page></net></pnml>"""
    
    output = run_deadlock_check(xml, capsys)
    assert "DEADLOCK FOUND" in output

# --- TC 4.3: Deadlock do Tranh chấp tài nguyên (Resource Sharing) ---
# A giữ R1 cần R2. B giữ R2 cần R1. -> Kẹt cứng.
def test_resource_deadlock(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p_a1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p_b1"><initialMarking><text>1</text></initialMarking></place>
      <place id="r1"><initialMarking><text>1</text></initialMarking></place>
      <place id="r2"><initialMarking><text>1</text></initialMarking></place>
      <place id="p_a2"/><place id="p_b2"/><place id="p_done"/>

      <transition id="t_a_get_r1"/><transition id="t_b_get_r2"/>
      <transition id="t_a_get_r2"/><transition id="t_b_get_r1"/>

      <arc source="p_a1" target="t_a_get_r1"/><arc source="r1" target="t_a_get_r1"/><arc source="t_a_get_r1" target="p_a2"/>
      <arc source="p_a2" target="t_a_get_r2"/><arc source="r2" target="t_a_get_r2"/><arc source="t_a_get_r2" target="p_done"/>

      <arc source="p_b1" target="t_b_get_r2"/><arc source="r2" target="t_b_get_r2"/><arc source="t_b_get_r2" target="p_b2"/>
      <arc source="p_b2" target="t_b_get_r1"/><arc source="r1" target="t_b_get_r1"/><arc source="t_b_get_r1" target="p_done"/>
    </page></net></pnml>"""
    
    output = run_deadlock_check(xml, capsys)
    assert "DEADLOCK FOUND" in output