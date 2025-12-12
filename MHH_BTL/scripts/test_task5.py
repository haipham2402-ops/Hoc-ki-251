import pytest
import os
import tempfile
from run import PetriNetParser, ReachabilityGraph, Optimizer

# --- Helper: Tạo file PNML ---
def create_temp_pnml(content):
    fd, path = tempfile.mkstemp(suffix=".pnml", text=True)
    with os.fdopen(fd, 'w') as f:
        f.write(content)
    return path

# --- Helper: Chạy Optimizer và bắt Output ---
def run_optimization(xml_content, weights, capsys):
    path = create_temp_pnml(xml_content)
    try:
        # 1. Parse & Build Reachable Set (Dùng Task 2)
        parser = PetriNetParser(path)
        parser.parse()
        bfs = ReachabilityGraph(parser)
        all_states_list = bfs.compute_reachability()
        
        # Chuyển đổi sang set of tuples (cho giống logic main)
        reachable_set = set()
        for m in all_states_list:
            reachable_set.add(tuple(sorted(m.items())))
            
        # 2. Run Optimizer
        opt = Optimizer(parser, reachable_set)
        opt.run(weights)
        
        # 3. Lấy kết quả in ra
        captured = capsys.readouterr()
        return captured.out
    finally:
        os.remove(path)

# --- TC 5.1: Tối ưu hóa cơ bản (Maximize) ---
# Mạng: P1 -> T1 -> P2.
# Trạng thái: {P1:1} hoặc {P2:1}
# Trọng số: P1=10, P2=50.
# Kỳ vọng: Chọn {P2:1} vì 50 > 10.
def test_optimize_simple(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
    </page></net></pnml>"""
    
    weights = {'p1': 10, 'p2': 50}
    
    output = run_optimization(xml, weights, capsys)
    
    # Kiểm tra output tìm thấy giá trị lớn nhất
    assert "Best Value: 50" in output
    # Kiểm tra trạng thái được chọn chứa p2
    assert "p2:1" in output

# --- TC 5.2: Trọng số âm (Negative Weights / Penalty) ---
# Mạng xung đột: P1 -> P2 (Thưởng) HOẶC P1 -> P3 (Phạt)
# Weights: P2 = 100, P3 = -50.
# Kỳ vọng: Chọn nhánh P2.
def test_optimize_negative(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/><place id="p3"/>
      <transition id="t1"/><transition id="t2"/>
      <arc source="p1" target="t1"/><arc source="t1" target="p2"/>
      <arc source="p1" target="t2"/><arc source="t2" target="p3"/>
    </page></net></pnml>"""
    
    weights = {'p2': 100, 'p3': -50}
    
    output = run_optimization(xml, weights, capsys)
    assert "Best Value: 100" in output

# --- TC 5.3: Tổ hợp Token (Combination) ---
# Mạng Fork: P1 -> {P2, P3}.
# Trạng thái cuối: {P2:1, P3:1}
# Weights: P2=10, P3=20.
# Kỳ vọng: Value = 10*1 + 20*1 = 30.
def test_optimize_fork(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/><place id="p3"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/><arc source="t1" target="p3"/>
    </page></net></pnml>"""
    
    weights = {'p2': 10, 'p3': 20}
    
    output = run_optimization(xml, weights, capsys)
    assert "Best Value: 30" in output

# --- TC 5.4: Trọng số mặc định (Default Weights) ---
# Kiểm tra xem nếu không set weight cho P3, nó có mặc định là 0 (hoặc giá trị trong code) không.
# Trong run.py, bạn đang để logic: weights.get(p_id, 0).
def test_optimize_default(capsys):
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
    </page></net></pnml>"""
    
    # Chỉ set weight cho P1, P2 không set (mặc định 0)
    weights = {'p1': 100} 
    
    output = run_optimization(xml, weights, capsys)
    
    # Trạng thái 1: {P1:1} -> Val = 100
    # Trạng thái 2: {P2:1} -> Val = 0 (default)
    # Kỳ vọng: Best Value là 100 (ở trạng thái đầu)
    assert "Best Value: 100" in output
    assert "p1:1" in output