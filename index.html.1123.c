<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>MetaMath Tutor</title>

    <!-- MathJax for LaTeX rendering -->
    <script src="https://polyfill.io/v3/polyfill.min.js?features=es6"></script>
    <script id="MathJax-script" async
        src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js">
    </script>

    <style>
        body {
            font-family: "Inter", Arial, sans-serif;
            background: #f0f2f5;
            padding: 20px;
            margin: 0;
        }

        h2 {
            text-align: center;
            margin-bottom: 15px;
            font-weight: 600;
        }

        #chatbox {
            border: 1px solid #ddd;
            padding: 15px;
            height: 430px;
            overflow-y: auto;
            margin-bottom: 15px;
            background: #ffffff;
            border-radius: 12px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.06);
        }

        .bubble {
            padding: 10px 14px;
            margin: 10px 0;
            border-radius: 14px;
            max-width: 80%;
            line-height: 1.5;
            animation: fadeIn 0.25s ease-in-out;
        }

        .user {
            background: #d9ecff;
            margin-left: auto;
            border-bottom-right-radius: 4px;
            text-align: left;
        }

        .bot {
            background: #f3e8ff;
            border-bottom-left-radius: 4px;
        }

        /* Typing indicator */
        .typing {
            display: inline-block;
            margin: 10px 0;
            padding: 8px 14px;
            background: #eee;
            border-radius: 10px;
            font-size: 14px;
            color: #777;
        }

        #input-container {
            display: flex;
            gap: 10px;
        }

        #inputbox {
            flex: 1;
            min-height: 50px;
            max-height: 150px;
            padding: 10px;
            font-size: 15px;
            border-radius: 10px;
            border: 1px solid #ccc;
            resize: vertical;
        }

        button {
            background: #0066ff;
            color: white;
            border: none;
            padding: 12px 18px;
            border-radius: 10px;
            cursor: pointer;
            font-size: 15px;
            font-weight: 500;
        }

        button:hover {
            background: #0050d8;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(4px); }
            to { opacity: 1; transform: translateY(0); }
        }

        /* Static Quick Prompts */
        #quick-prompts {
            display: flex;
            flex-wrap: wrap;
            gap: 8px;
            margin-bottom: 12px;
        }

        .qp {
            background: #eef3ff;
            color: #334;
            border: 1px solid #ccd5ff;
            padding: 6px 10px;
            border-radius: 14px;
            font-size: 14px;
            cursor: pointer;
            transition: 0.15s;
        }

        .qp:hover {
            background: #dfe7ff;
            border-color: #b9c7ff;
        }

        /* Adaptive Prompts */
        #adaptive-prompts {
            display: flex;
            flex-wrap: wrap;
            gap: 8px;
            margin-top: 10px;
            margin-bottom: 15px;
        }

        .ap {
            background: #fff7d1;
            color: #554400;
            border: 1px solid #ffe88b;
            padding: 6px 10px;
            border-radius: 12px;
            font-size: 14px;
            cursor: pointer;
            transition: 0.15s;
        }

        .ap:hover {
            background: #ffefad;
        }
    </style>
</head>

<body>

<h2>MetaMath Tutor</h2>

<div id="chatbox"></div>

<!-- STATIC QUICK PROMPTS -->
<div id="quick-prompts">
    <button class="qp" onclick="insertPrompt('I realized that…')">I realized that…</button>
    <button class="qp" onclick="insertPrompt('I found it difficult when…')">I found it difficult when…</button>
    <button class="qp" onclick="insertPrompt('My understanding changed when…')">My understanding changed when…</button>
    <button class="qp" onclick="insertPrompt('A question I still have is…')">A question I still have is…</button>
    <button class="qp" onclick="insertPrompt('The strategy that helped me most was…')">The strategy that helped me most was…</button>
</div>

<!-- ADAPTIVE PROMPTS -->
<h3 style="margin: 0 0 6px 0; font-size: 15px; font-weight: 500;">Suggested prompts</h3>
<div id="adaptive-prompts"></div>

<div id="input-container">
    <textarea id="inputbox" placeholder="Type your reflection..." ></textarea>
    <button onclick="sendMessage()">Send</button>
</div>

<script>
// -------------------------------------------------------
// --- Anonymous User ID Generation ---
// -------------------------------------------------------
function generateAnonymousId() {
    const random = Math.random().toString(36).substring(2, 10);
    return "user_" + random;
}

let anonymousUserId = localStorage.getItem("metamath-anon-id");
if (!anonymousUserId) {
    anonymousUserId = generateAnonymousId();
    localStorage.setItem("metamath-anon-id", anonymousUserId);
}
console.log("Anonymous ID:", anonymousUserId);

// --- New Session ID each visit ---
function generateSessionId() {
    return "sess_" + Math.random().toString(36).substring(2, 10);
}
let sessionId = generateSessionId();
console.log("Session ID:", sessionId);

// --- Get Lesson ID from URL ---
const params = new URLSearchParams(window.location.search);
const lessonId = params.get("lesson") || "unknown_lesson";
console.log("Lesson:", lessonId);

// -------------------------------------------------------
// --- Add Chat Bubble ---
// -------------------------------------------------------
function addBubble(text, className) {
    const chat = document.getElementById("chatbox");

    const div = document.createElement("div");
    div.className = "bubble " + className;
    div.innerHTML = text;

    chat.appendChild(div);
    chat.scrollTop = chat.scrollHeight;

    if (window.MathJax) MathJax.typesetPromise();
}

// -------------------------------------------------------
// --- Typing Indicator ---
// -------------------------------------------------------
function showTyping() {
    const chat = document.getElementById("chatbox");
    const div = document.createElement("div");
    div.id = "typing";
    div.className = "typing";
    div.textContent = "Metamath Tutor is typing...";
    chat.appendChild(div);
    chat.scrollTop = chat.scrollHeight;
}

function hideTyping() {
    const el = document.getElementById("typing");
    if (el) el.remove();
}

// -------------------------------------------------------
// --- Insert Static or Adaptive Prompt ---
// -------------------------------------------------------
function insertPrompt(text) {
    const input = document.getElementById("inputbox");
    input.value = text + " ";
    input.focus();
}

// -------------------------------------------------------
// --- Render Adaptive Prompts ---
// -------------------------------------------------------
function renderAdaptivePrompts(list) {
    const box = document.getElementById("adaptive-prompts");
    box.innerHTML = "";

    if (!list || list.length === 0) return;

    list.forEach(item => {
        const btn = document.createElement("button");
        btn.className = "ap";
        btn.textContent = item;
        btn.onclick = () => insertPrompt(item);
        box.appendChild(btn);
    });
}

// -------------------------------------------------------
// --- Send Message ---
// -------------------------------------------------------
function sendMessage() {
    const input = document.getElementById("inputbox");
    const msg = input.value.trim();
    if (!msg) return;

    addBubble(msg, "user");
    input.value = "";

    showTyping();

    fetch("https://metamath-backend-1.onrender.com/chat", {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify({
            message: msg,
            lesson_id: lessonId,
            user_id: anonymousUserId,
            session_id: sessionId
        })
    })
    .then(response => response.json())
    .then(data => {
        hideTyping();
        addBubble(data.reply, "bot");

        // NEW: Show Adaptive Suggestions
        if (data.suggestions) {
            renderAdaptivePrompts(data.suggestions);
        }
    })
    .catch(err => {
        hideTyping();
        addBubble("⚠️ Error: Could not reach the server.", "bot");
    });
}
</script>

</body>
</html>

