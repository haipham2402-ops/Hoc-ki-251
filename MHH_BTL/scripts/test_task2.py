import pytest
import os
import tempfile
from run import PetriNetParser, ReachabilityGraph

# --- Helper: Tạo file và chạy thuật toán ---
def run_algo(xml_content, method):
    fd, path = tempfile.mkstemp(suffix=".pnml", text=True)
    with os.fdopen(fd, 'w') as f:
        f.write(xml_content)
    try:
        parser = PetriNetParser(path)
        parser.parse()
        solver = ReachabilityGraph(parser)
        return solver.compute_reachability(method=method)
    finally:
        os.remove(path)

# --- TEST SUITE: Chạy chung cho cả BFS và DFS ---
# Mỗi hàm test sẽ được chạy 2 lần: 1 lần với 'BFS', 1 lần với 'DFS'
@pytest.mark.parametrize("method", ["BFS", "DFS"])
def test_linear_chain(method):
    """P1 -> T1 -> P2: Cả BFS và DFS đều phải tìm ra 2 trạng thái"""
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
    </page></net></pnml>"""
    
    results = run_algo(xml, method)
    assert len(results) == 2

@pytest.mark.parametrize("method", ["BFS", "DFS"])
def test_fork_branching(method):
    """Rẽ nhánh: T1 sinh token cho P2 và P3"""
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <place id="p3"/>
      <transition id="t1"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
      <arc source="t1" target="p3"/>
    </page></net></pnml>"""
    
    results = run_algo(xml, method)
    assert len(results) == 2
    # Kiểm tra trạng thái cuối có cả p2 và p3
    final = results[-1]
    assert final['p2'] == 1 and final['p3'] == 1

@pytest.mark.parametrize("method", ["BFS", "DFS"])
def test_simple_cycle(method):
    """Vòng lặp: P1 -> T1 -> P2 -> T2 -> P1. Phải dừng, không lặp vô hạn"""
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/>
      <transition id="t1"/>
      <transition id="t2"/>
      <arc source="p1" target="t1"/>
      <arc source="t1" target="p2"/>
      <arc source="p2" target="t2"/>
      <arc source="t2" target="p1"/>
    </page></net></pnml>"""
    
    results = run_algo(xml, method)
    assert len(results) == 2

# --- TEST CASE ĐẶC BIỆT: So sánh sự khác biệt BFS vs DFS ---
def test_bfs_vs_dfs_order():
    """
    Mô hình kim tự tháp để kiểm tra thứ tự duyệt:
          P1 (Root)
         /  \
       T1    T2
       /      \
      P2      P3
      |       |
      T3      T4
      |       |
      P4      P5
    
    - BFS (Chiều rộng): Nên tìm thấy P2, P3 trước khi thấy P4, P5.
    - DFS (Chiều sâu): Nên đi hết một nhánh (VD: P2->P4) trước khi sang nhánh kia (P3).
    """
    xml = """<?xml version="1.0"?>
    <pnml><net><page>
      <place id="p1"><initialMarking><text>1</text></initialMarking></place>
      <place id="p2"/><place id="p3"/><place id="p4"/><place id="p5"/>
      <transition id="t1"/><transition id="t2"/><transition id="t3"/><transition id="t4"/>
      
      <arc source="p1" target="t1"/><arc source="t1" target="p2"/>
      <arc source="p2" target="t3"/><arc source="t3" target="p4"/>
      
      <arc source="p1" target="t2"/><arc source="t2" target="p3"/>
      <arc source="p3" target="t4"/><arc source="t4" target="p5"/>
    </page></net></pnml>"""
    
    # 1. Chạy BFS
    bfs_states = run_algo(xml, "BFS")
    # BFS List order logic: [Root, Level1_A, Level1_B, Level2_A, Level2_B]
    # Trạng thái số 1 và 2 (index 1, 2) phải là lớp con trực tiếp (có token ở p2 hoặc p3)
    # Trạng thái sâu hơn (p4, p5) phải ở cuối.
    # Note: visited_order[0] là Root.
    
    # Kiểm tra BFS: Các trạng thái đầu tiên sau root phải là độ sâu 1 (P2 hoặc P3)
    depth_1_nodes = 0
    for i in [1, 2]: # Xem 2 node tiếp theo
        m = bfs_states[i]
        if m.get('p2') == 1 or m.get('p3') == 1:
            depth_1_nodes += 1
    assert depth_1_nodes == 2, "BFS phải duyệt hết lớp con (P2, P3) trước"

    # 2. Chạy DFS
    dfs_states = run_algo(xml, "DFS")
    # DFS List order logic: [Root, Branch1_L1, Branch1_L2, Branch2_L1...]
    # Trạng thái thứ 2 (index 2) phải là cháu (P4 hoặc P5) thay vì con của nhánh kia.
    
    # Lấy node thứ 2 (index 1) - Giả sử đi vào nhánh trái (P2)
    # Thì node thứ 3 (index 2) PHẢI là con của nó (P4), CHỨ KHÔNG ĐƯỢC là nhánh phải (P3)
    
    second_node = dfs_states[1]
    third_node = dfs_states[2]
    
    if second_node.get('p2') == 1:
        # Nếu đã đi nhánh P2, node tiếp theo phải là P4 (sâu hơn)
        assert third_node.get('p4') == 1, "DFS phải đi sâu xuống P4 trước khi qua nhánh P3"
    elif second_node.get('p3') == 1:
        # Nếu đã đi nhánh P3, node tiếp theo phải là P5
        assert third_node.get('p5') == 1, "DFS phải đi sâu xuống P5 trước khi qua nhánh P2"