import pytest
import os
import tempfile
import sys
# Import class từ run.py
from run import PetriNetParser, ReachabilityGraph, SymbolicReachability, HAS_BDD

# --- Helper: Tạo file PNML ---
def create_temp_pnml(content):
    fd, path = tempfile.mkstemp(suffix=".pnml", text=True)
    with os.fdopen(fd, 'w') as f:
        f.write(content)
    return path

# --- Helper: Chạy BDD và trả về số lượng trạng thái ---
def run_symbolic(xml_content):
    if not HAS_BDD:
        pytest.skip("Thư viện 'dd' chưa được cài đặt. Bỏ qua test Task 3.")
        
    path = create_temp_pnml(xml_content)
    try:
        parser = PetriNetParser(path)
        parser.parse()
        solver = SymbolicReachability(parser)
        return solver.run()
    finally:
        os.remove(path)

# --- Test Case 1: Chuỗi tuyến tính (Linear Chain) ---
# P1 -> T1 -> P2. (2 trạng thái: {p1:1}, {p2:1})
def test_bdd_linear_chain():
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
    </page></net></pnml>"""
    
    count = run_symbolic(xml)
    assert count == 2, "BDD phải đếm được 2 trạng thái"

# --- Test Case 2: Rẽ nhánh đồng thời (Fork) ---
# P1 -> T1 -> (P2, P3). (2 trạng thái: {p1}, {p2, p3})
def test_bdd_fork():
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/><place id="p3"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
      <arc source="t1" target="p3"/>
    </page></net></pnml>"""
    
    count = run_symbolic(xml)
    assert count == 2

# --- Test Case 3: Xung đột (Conflict) ---
# P1 -> T1 -> P2  HOẶC  P1 -> T2 -> P3
# (3 trạng thái: {p1}, {p2}, {p3})
def test_bdd_conflict():
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/><place id="p3"/>
      <transition id="t1"/><transition id="t2"/>
      <arc source="p1" target="t1"/><arc source="t1" target="p2"/>
      <arc source="p1" target="t2"/><arc source="t2" target="p3"/>
    </page></net></pnml>"""
    
    count = run_symbolic(xml)
    assert count == 3

# --- Test Case 4: Vòng lặp (Cycle) ---
# P1 -> T1 -> P2 -> T2 -> P1 (2 trạng thái lặp lại)
def test_bdd_cycle():
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/><transition id="t2"/>
      <arc source="p1" target="t1"/><arc source="t1" target="p2"/>
      <arc source="p2" target="t2"/><arc source="t2" target="p1"/>
    </page></net></pnml>"""
    
    count = run_symbolic(xml)
    assert count == 2

# --- Test Case 5: So sánh Consistency (Explicit vs Symbolic) ---
# Chạy cả Task 2 và Task 3 trên cùng 1 model phức tạp hơn và so sánh kết quả.
def test_compare_explicit_vs_symbolic():
    if not HAS_BDD:
        pytest.skip("No BDD lib")
        
    # Mô hình: P1 -> T1 -> {P2, P3} -> T2 -> P4 (Join)
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/><place id="p3"/><place id="p4"/>
      <transition id="t1"/><transition id="t2"/>
      
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/><arc source="t1" target="p3"/>
      
      <arc source="p2" target="t2"/><arc source="p3" target="t2"/>
      <arc source="t2" target="p4"/>
    </page></net></pnml>"""
    
    path = create_temp_pnml(xml)
    try:
        parser = PetriNetParser(path)
        parser.parse()
        
        # 1. Chạy Explicit (Task 2)
        explicit_solver = ReachabilityGraph(parser)
        explicit_states = explicit_solver.compute_reachability()
        explicit_count = len(explicit_states)
        
        # 2. Chạy Symbolic (Task 3)
        symbolic_solver = SymbolicReachability(parser)
        symbolic_count = symbolic_solver.run()
        
        print(f"Explicit: {explicit_count}, Symbolic: {symbolic_count}")
        assert explicit_count == symbolic_count, "Kết quả Explicit và Symbolic phải giống nhau"
        
        # Kỳ vọng: 3 trạng thái
        # 1. {p1}
        # 2. {p2, p3} (sau t1)
        # 3. {p4} (sau t2 - vì t2 cần cả p2, p3 nên t2 bắn được)
        assert explicit_count == 3
        
    finally:
        os.remove(path)