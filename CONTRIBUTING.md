# Contributing & Engineering Methodology

> Note: This repository reflects an exploratory Proof of Concept (PoC) / MVP developed to validate hardware integration on legacy silicon. The core architecture is being systematically redesigned under formal system engineering principles.

## 1. Core Philosophy: Engineering as Applied Science

At ELI_galileo, we **reject ad-hoc hacking and trial-and-error development**. Engineering is the **scientific method applied to creation**. While natural sciences use the scientific method for **discovery**, engineering uses an **isomorphic application** of the same logical rigor for **creation**. **Any attempt to bypass, subvert, or break the scientific method within this project will be indefinitely vetoed.**

We operate under both formal expressions of the scientific method:

### A. The Scientific Method of Discovery (Understanding Reality)
1. **Observation**: Identifying a physical or operational phenomenon.
2. **Hypothesis**: Proposing a model or explanation for the phenomenon.
3. **Experiment Design**: Setting up controlled conditions to test the model.
4. **Experimentation**: Execution and data gathering.
5. **Data Consistency Verification**: Ensuring experimental data is sound and repeatable.
6. **Hypothesis Validation**: Confirming or refuting the proposed model.
7. **Natural Law / Theory**: Establishing reliable principles.

### B. The Scientific Method of Creation (Engineering & Synthesis)
1. **Requirements (Hypothesis of Need)**: Define the problem to solve and the constraints to meet (e.g., Phase 0/A MDD & URD).
2. **Behavioral Design**: Architect the system behavior to satisfy the requirements without violating physical or logical boundaries (SRS & Architecture).
3. **Implementation/Manufacturing Procedure**: Formulate the rigorous, reproducible steps required to build the artifact (coding standards, MISRA-C, Power of 10, build pipelines).
4. **Implementation / Execution**: Fabricate or code the artifact according to the procedure.
5. **Procedure Verification**: Confirm that the artifact was built *exactly* as specified by the procedure (static analysis, unit testing).
6. **Behavioral Validation**: Prove that the artifact's behavior matches the original requirements (via SITL and HITL testing).
7. **Prototype / Product Integration**: Deploy the validated artifact into its operational plant environment (e.g., UAV flight integration).

---

## 2. Standards as Guardrails

While the scientific method provides the underlying logic, **industrial and aerospace standards** (e.g., **ECSS**, **MISRA-C**, **NASA Power of 10**) serve as our **operational guardrails**. These standards:

- Eliminate ambiguity.
- Enforce determinism.
- Prevent systemic errors before code touches silicon.

---

## 3. Repository Workflow Procedure

To maintain the integrity of this methodology, all contributions and changes must follow a **rigorous lifecycle** mirroring the scientific method of creation:

### 1. **Issue Tracking**
- **No change**, refactoring, or feature is implemented **without a prior formal Issue** detailing its rationale and traceability.
- All Issues must include:
  - A **clear and formal description** of the problem or enhancement.
  - **Traceability to requirements** (if applicable).
  - **Impact analysis** on system behavior, safety, and performance.
- **Lifecycle Transition:** Sub-issues (Requirements, Design, Implementation) are reviewed, validated, and **closed exclusively via Pull Request (PR)** merge.

```mermaid
graph TD
    classDef req fill:#d4edda,stroke:#28a745,stroke-width:2px,color:#155724;
    classDef des fill:#cce5ff,stroke:#004085,stroke-width:2px,color:#004085;
    classDef imp fill:#fff3cd,stroke:#ffc107,stroke-width:2px,color:#856404;
    classDef vv fill:#f8d7da,stroke:#dc3545,stroke-width:2px,color:#721c24;
    classDef core fill:#e2e3e5,stroke:#383d41,stroke-width:2px,color:#383d41;

    A([Observe a Need]) --> B{Is it necessary for the project?}
    B -->|No| C[Do nothing]
    B -->|Yes| W[Create Main Issue]

    W --> D[Create Requirements Issue]:::req
    D --> E[Open Requirements Branch]:::req
    E --> F[Define Formal Requirements]:::req
    F --> G[Merge PR & Close Requirements Issue]:::req

    G --> H[Create Design Issue]:::des
    H --> I[Open Design Branch]:::des
    I --> J[System Design]:::des
    J --> K[Merge PR & Close Design Issue]:::des

    K --> L[Create Implementation Issue]:::imp
    L --> M[Open Implementation Branch]:::imp
    M --> N[Define Implementation Procedure & Code Implementation]:::imp
    N --> O[Tests and Static Analysis]:::imp
    O --> P{Does implementation comply with procedure?}:::imp
    P -->|No| N
    P -->|Yes| Q[Merge PR & Close Implementation Issue]:::imp

    Q --> R[Create Verification Issue]:::vv
    R --> S[Open Verification Branch]:::vv
    S --> T[Unit Tests, SIL, HIL]:::vv
    T --> U{Does design satisfy requirements?}:::vv
    U -->|No - Design Flaw| X[Merge PR & Close Verification Issue]:::vv
    X --> I
    U -->|Yes - Pass| Y[Merge PR & Close Verification Issue]:::vv

    Y --> Z[Create Validation Issue]:::vv
    Z --> AA[Open Validation Branch]:::vv
    AA --> AB[System Validation]:::vv
    AB --> AC{Does it solve the problem?}:::vv
    AC -->|No - Requirement Flaw| AD[Merge PR & Close Validation Issue]:::vv
    AD --> E
    AC -->|Yes - Pass| AE[Merge PR & Close Validation Issue]:::vv
    AE --> AF([Close Main Issue])
```

### 2. **Branching Strategy**
- **Feature/Task branches** must stem from ongoing baselines using **descriptive names**, such as:
  - `docs/urd_draft`
  - `feat/sensor-fusion`
  - `bugfix/i2c-daemon-crash`
- Branches must be **clearly documented** in the associated Issue.

### 3. **Documentation-Driven Engineering**
- **Requirements and design specifications** must be updated **before** code implementation.
- **Traceability links** must be maintained between requirements and verification artifacts (e.g., unit tests, static analysis rules).

### 4. **Code Quality Gates**
- **Zero dynamic memory allocation**: `malloc`, `free`, or any heap usage is **strictly prohibited**.
- **Full compliance** with static analysis rules and coding guidelines.
- **Verification through reproducible testing procedures**.

### 5. **Review & Merge**
- All **code and documentation** are subject to **peer/architectural review** before merging into the main line.
- **No merge** is allowed without:
  - A successful **CI pipeline** (static analysis, unit tests, packaging).
  - **Traceable documentation** updates.
  - **Compliance with standards** (MISRA-C, ECSS, etc.).
  - Every Pull Request for Implementation, Verification, or Validation must include the corresponding execution logs, test reports, or traceability matrices as mandatory artifacts before review and merge.

---

## 4. Commit Message Guidelines

Standardized commit messages are enforced to ensure clarity and traceability. The use of **Conventional Commits** is encouraged.

**Format:** `<type>(<scope>): <description>`

- **type**:
  - `feat`: A new feature or enhancement.
  - `fix`: A bug fix.
  - `docs`: Documentation only changes.
  - `style`: Changes that do not affect the meaning of the code (whitespace, formatting, etc.).
  - `refactor`: A code change that neither fixes a bug nor adds a feature.
  - `test`: Adding a missing test or correcting existing tests.
  - `chore`: Maintenance tasks, build system updates, etc.
- **scope** (optional): The specific module affected (e.g., `i2c-daemon`, `buildroot-script`, `docs`).
- **description**: A short, imperative tense description of the change.

**Examples:**
- `feat(i2c-daemon): Add support for the new sensor model`
- `docs(readme): Update CI badge URL`
- `fix(buildroot-script): Ensure .deb artifact is downloaded correctly`

---

## 5. Pull Request (PR) Checklist

Before submitting your Pull Request, please ensure the following:

- [ ] **Code Builds Successfully:** Your changes compile correctly using the cross-compilation toolchain.
- [ ] **CI Pipeline passes:** The GitHub Actions workflow must pass all steps (static analysis, unit tests, packaging) successfully.
- [ ] **Unit Tests Added/Passed:** You have added relevant unit tests for new features/fixes, and all existing tests pass.
- [ ] **Documentation Updated:** `README.md` or other relevant documentation is updated to reflect your changes.
- [ ] **MISRA Compliance:** C/C++ code adheres to project programming guidelines (checked by CppCheck in CI).
- [ ] **Traceability Links:** All changes are traceable to formal requirements or design documents.
- [ ] **Binary Audit Completed:** Any example or reference implementation in the repository has passed a static/WCET audit.

---

## 6. Reporting Issues

If you find a bug or have a feature request, please open a new issue with the following information:

- **Title**: Clear and concise.
- **Description**: Detailed and traceable to a requirement or design document.
- **Impact Analysis**: Potential system impact, safety, or performance considerations.
- **Reproduction Steps**: If applicable, provide steps to reproduce the issue.

Thank you for your contribution!