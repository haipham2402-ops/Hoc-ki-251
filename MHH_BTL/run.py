import xml.etree.ElementTree as ET
import sys
import os
from collections import deque, defaultdict

# --- THƯ VIỆN BDD (Task 3) ---
try:
    from dd.autoref import BDD
    HAS_BDD = True
except ImportError:
    HAS_BDD = False
    print("Failed Task 3")

# --- THƯ VIỆN ILP (Task 4 & 5)
try:
    import pulp
    HAS_PULP = True
except ImportError:
    HAS_PULP = False
    print("Failed Task 4 5")

# ==========================================
# TASK 1: PNML PARSER
# ==========================================
class PetriNetParser:
    def __init__(self, file_path):
        self.file_path = file_path
        self.places = {}      # {id: {name, tokens}}
        self.transitions = {} # {id: {name}}
        self.arcs = []        
        self.pre_set = defaultdict(list)  
        self.post_set = defaultdict(list) 

    def parse(self):
        if not os.path.exists(self.file_path):
            print(f"File not found: {self.file_path}")
            return False
        try:
            tree = ET.parse(self.file_path)
            root = tree.getroot()
            for elem in root.iter():
                tag = elem.tag.split('}')[-1] if '}' in elem.tag else elem.tag
                if tag == 'place':
                    p_id = elem.get('id'); p_name = p_id; tokens = 0
                    for child in elem.iter():
                        c_tag = child.tag.split('}')[-1]
                        if c_tag == 'name':
                            for sub in child.iter(): 
                                if sub.text and sub.text.strip(): p_name = sub.text.strip()
                        if c_tag == 'initialMarking':
                            for sub in child.iter():
                                if sub.text and sub.text.strip().isdigit(): tokens = int(sub.text.strip())
                    self.places[p_id] = {'name': p_name, 'tokens': tokens}
                elif tag == 'transition':
                    t_id = elem.get('id'); t_name = t_id
                    for child in elem.iter():
                        c_tag = child.tag.split('}')[-1]
                        if c_tag == 'name':
                            for sub in child.iter(): 
                                if sub.text and sub.text.strip(): t_name = sub.text.strip()
                    self.transitions[t_id] = {'name': t_name}
                elif tag == 'arc':
                    src = elem.get('source'); tgt = elem.get('target')
                    self.arcs.append({'id': elem.get('id'), 'source': src, 'target': tgt})
            
            # Build Cache
            for arc in self.arcs:
                if arc['target'] in self.transitions: self.pre_set[arc['target']].append(arc['source'])
                if arc['source'] in self.transitions: self.post_set[arc['source']].append(arc['target'])

            if not self.places: return False
            return True
        except Exception as e:
            print(f"Parse error: {e}")
            return False

    def verify_consistency(self):
        return bool(self.arcs)

# ==========================================
# TASK 2: EXPLICIT REACHABILITY
# ==========================================
class ReachabilityGraph:
    def __init__(self, parser):
        self.parser = parser
        self.visited_set = set() # Set of tuples (hashable markings)

    def get_enabled(self, marking):
        enabled = []
        for t in self.parser.transitions:
            inputs = self.parser.pre_set[t]
            if all(marking.get(p, 0) >= 1 for p in inputs):
                enabled.append(t)
        return enabled

    def fire(self, marking, t):
        new_m = marking.copy()
        for p in self.parser.pre_set[t]: new_m[p] -= 1
        for p in self.parser.post_set[t]: new_m[p] = new_m.get(p, 0) + 1
        return new_m

    # --- ĐÃ SỬA TÊN HÀM TẠI ĐÂY (compute -> compute_reachability) ---
    def compute_reachability(self, method='BFS'):
        print(f"\n[Task 2] Computing Explicit Reachability ({method})...")
        m0 = {pid: d['tokens'] for pid, d in self.parser.places.items()}
        
        q = deque([m0])
        self.visited_set = set([tuple(sorted(m0.items()))])
        
        # SỬA 1: Không thêm m0 vào đây nữa
        visited_order = [] 

        while q:
            if method == 'BFS':
                curr = q.popleft()
            else:
                curr = q.pop()
            
            # SỬA 2: Thêm vào danh sách KHI ĐƯỢC LẤY RA (Processing Order)
            visited_order.append(curr)

            for t in self.get_enabled(curr):
                next_m = self.fire(curr, t)
                next_tup = tuple(sorted(next_m.items()))
                
                if next_tup not in self.visited_set:
                    self.visited_set.add(next_tup)
                    # SỬA 3: Xóa dòng append ở đây đi
                    # visited_order.append(next_m) 
                    q.append(next_m)
        
        print(f"Task 2 Done. Reachable states: {len(self.visited_set)}")
        return visited_order
# ==========================================
# TASK 3: SYMBOLIC (BDD)
# ==========================================
class SymbolicReachability:
    def __init__(self, parser):
        self.parser = parser
        if HAS_BDD:
            self.bdd = BDD()
            self.x_vars = [f'x_{i}' for i in range(len(parser.places))]
            self.y_vars = [f'y_{i}' for i in range(len(parser.places))]
            self.bdd.declare(*self.x_vars); self.bdd.declare(*self.y_vars)
            self.p_list = list(parser.places.keys())
            self.p_idx = {p: i for i, p in enumerate(self.p_list)}

    def run(self):
        if not HAS_BDD: return 0
        print("\n[Task 3] Computing Symbolic Reachability (BDD)...")
        
        # Initial State
        parts = [f"x_{i}" if self.parser.places[p]['tokens']>0 else f"~x_{i}" for i, p in enumerate(self.p_list)]
        S_curr = self.bdd.add_expr(" & ".join(parts))

        # Transition Relation
        T_total = self.bdd.false
        for t in self.parser.transitions:
            ins = [self.p_idx[p] for p in self.parser.pre_set[t]]
            outs = [self.p_idx[p] for p in self.parser.post_set[t]]
            en_expr = [f"x_{i}" for i in ins]
            ch_expr = []
            for i in range(len(self.p_list)):
                if i in ins and i not in outs: ch_expr.append(f"~y_{i}")
                elif i in outs: ch_expr.append(f"y_{i}")
                else: ch_expr.append(f"(x_{i} <-> y_{i})")
            
            full = " & ".join(en_expr + ch_expr) if (en_expr or ch_expr) else "True"
            T_total |= self.bdd.add_expr(full)

        # Fixpoint Loop
        while True:
            next_y = self.bdd.quantify(S_curr & T_total, self.x_vars, forall=False)
            rename = {y: x for x, y in zip(self.x_vars, self.y_vars)}
            next_x = self.bdd.let(rename, next_y)
            S_new = S_curr | next_x
            if S_new == S_curr: break
            S_curr = S_new

        count = int(self.bdd.count(S_curr, nvars=len(self.x_vars)))
        print(f"Task 3 Done. Count: {count}")
        return count

# ==========================================
# TASK 4: DEADLOCK DETECTION (Logic from DeadlockDetector.cpp)
# ==========================================
class DeadlockDetector:
    def __init__(self, parser, reachable_set):
        self.parser = parser
        self.reachable_set = reachable_set # Set of tuples from Task 2

    def run(self):
        if not HAS_PULP: return
        print("\n[Task 4] Deadlock Detection (Iterative ILP)...")

        # 1. Check trivial case (Transition without input)
        for t in self.parser.transitions:
            if not self.parser.pre_set[t]:
                print("   -> Trivial: Transition source exists => No Deadlock.")
                return

        # 2. ILP Setup (Maximize tokens heuristic like C++ code)
        prob = pulp.LpProblem("Deadlock", pulp.LpMaximize)
        M = {p: pulp.LpVariable(f"M_{p}", cat='Binary') for p in self.parser.places}
        prob += pulp.lpSum(M.values()) # Maximize tokens to find interesting states

        # 3. Deadlock Constraints: All transitions must be disabled
        # sum(M[p] for p in pre(t)) <= |pre(t)| - 1
        for t in self.parser.transitions:
            inputs = self.parser.pre_set[t]
            if inputs:
                prob += pulp.lpSum([M[p] for p in inputs]) <= len(inputs) - 1

        # 4. Iterative Loop (Solve -> Check -> Cut)
        iter_count = 0
        while True:
            iter_count += 1
            prob.solve(pulp.PULP_CBC_CMD(msg=0))
            
            if pulp.LpStatus[prob.status] != "Optimal":
                print("[Task 4] ILP Infeasible. No Deadlock possible.")
                return

            # Get candidate
            cand_dict = {}
            ones = []
            for p in self.parser.places:
                val = int(pulp.value(M[p]))
                cand_dict[p] = val
                if val == 1: ones.append(p)
            
            cand_tuple = tuple(sorted(cand_dict.items()))

            # Check reachability
            if cand_tuple in self.reachable_set:
                print(f"DEADLOCK FOUND (Iter {iter_count})!")
                dead_str = ", ".join([p for p in ones])
                print(f"   Marking: {{{dead_str}}} (All transitions disabled)")
                return
            else:
                # Add Cut (Spurious solution)
                # Constraint: sum(M[p] where p=1) <= count(1) - 1
                if ones:
                    prob += pulp.lpSum([M[p] for p in ones]) <= len(ones) - 1
                else:
                    prob += pulp.lpSum(M.values()) >= 1 # Block empty state

# ==========================================
# TASK 5: OPTIMIZATION (Logic from Optimizer.cpp)
# ==========================================
class Optimizer:
    def __init__(self, parser, reachable_set):
        self.parser = parser
        self.reachable_set = reachable_set

    def run(self, weights):
        print("\n[Task 5] Optimization over Reachable Set...")
        
        if not self.reachable_set:
            print("   -> Reachable set empty.")
            return

        best_val = -float('inf')
        best_marking = None

        # Iterate explicitly over reachable set (From C++ optimizeExplicit)
        for m_tuple in self.reachable_set:
            # m_tuple format: (('p1', 1), ('p2', 0)...)
            current_val = 0
            for p_id, token in m_tuple:
                try:
                    token_val = int(token)
                except ValueError:
                    token_val = 0
                
                if token_val > 0:
                    current_val += weights.get(p_id, 0) * token_val
            
            if current_val > best_val:
                best_val = current_val
                best_marking = m_tuple

        print(f"[Task 5] Best Value: {best_val}")
        
        # Pretty print best marking
        best_dict = dict(best_marking)
        active = [f"{k}:{v}" for k,v in best_dict.items() if v > 0]
        print(f"   Best Marking: {{{', '.join(active)}}}")

# ==========================================
# MAIN
# ==========================================
if __name__ == "__main__":
    filename = "xray.pnml"
    
    print(f"--- PROCESSING: {filename} ---")
    parser = PetriNetParser(filename)
    
    if parser.parse() and parser.verify_consistency():
        
        # Task 2
        solver = ReachabilityGraph(parser)
        all_states = solver.compute_reachability()
        reachable_set = all_states # Convert to set for fast lookup

        # Task 3
        if HAS_BDD:
            bdd_solver = SymbolicReachability(parser)
            count = bdd_solver.run()
            print(f"   -> Explicit: {len(all_states)} vs Symbolic: {count}")

        # Task 4
        if HAS_PULP:
            deadlock = DeadlockDetector(parser, reachable_set)
            deadlock.run()

        # Task 5
        # Define weights (Example: p4(done)=10, p1(wait)=-1)
        weights = {p: 1 for p in parser.places} # Default weight = 1
        # weights['p4'] = 100 # Uncomment to test custom weights
        
        opt = Optimizer(parser, reachable_set)
        opt.run(weights)