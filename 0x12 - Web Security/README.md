# Web security

Most real-world "hacking" today is web hacking, and it's a large share of CTFs
(the Root-Me *Web-Client* / *Web-Server* categories in the root TODO). This
chapter maps the main vulnerability classes: how each works, how to spot it in a
lab, and — just as importantly — how it's fixed.

> **Scope reminder.** Only test applications you own or are explicitly
> authorised to test (your own lab, a CTF target, a program with a written
> scope). Unauthorised testing of live sites is illegal.

## How the web works (the 30-second model)

A browser (client) sends an **HTTP request** to a server, which returns a
**response**. Almost every web vulnerability is some variant of: *the server (or
another user's browser) trusts attacker-controlled input it shouldn't.*

```
GET /profile?id=42 HTTP/1.1        <- method, path, query string
Host: example.com                   <- headers (Cookie, User-Agent, ...)
                                    <- blank line
                                    <- body (for POST/PUT)
```

HTTP is stateless, so apps track who you are with a **session cookie** (or a
token) sent on every request.

## Essential tooling

* **Browser DevTools** (F12) — inspect requests, cookies, storage, and the DOM.
* **[Burp Suite](https://portswigger.net/burp)** (Community edition is free) —
  an intercepting proxy that sits between your browser and the server so you can
  read and *modify* every request. The single most important web tool. OWASP
  **ZAP** is a free, open-source alternative.
* **`curl`** — scripting requests from the terminal:
  ```sh
  curl -i 'https://target/profile?id=42' -b 'session=...'    # -i shows headers, -b sends a cookie
  curl -X POST -d 'user=admin&pass=x' https://target/login
  ```
* **[ffuf](https://github.com/ffuf/ffuf)** / **gobuster** — brute-force hidden
  paths and parameters from a wordlist (content discovery).

## The main vulnerability classes

### Injection — SQL injection (SQLi)

User input is concatenated into a database query, so the input can change the
query's *structure*:

```python
# VULNERABLE: input becomes part of the SQL
query = "SELECT * FROM users WHERE name = '" + name + "'"
```

Supplying `name = ' OR '1'='1` turns the `WHERE` clause always-true, dumping or
bypassing logic. A single quote that produces a database error is the classic
first probe. Impact ranges from auth bypass to reading the whole database.

* **Find it (lab):** add a `'`, observe errors or changed behaviour; confirm
  with boolean (`' AND '1'='1` vs `' AND '1'='2`) or time-based
  (`' AND SLEEP(5)-- `) tests. `sqlmap -u '<url>' --batch` automates detection
  and extraction.
* **Fix:** **parameterised queries / prepared statements** (let the driver bind
  values, never string-concatenate). Least-privilege DB accounts limit the blast
  radius.

### Cross-site scripting (XSS)

The server reflects or stores user input into a page without encoding it, so the
input runs as JavaScript in *another user's* browser — letting an attacker steal
their session, act as them, or deface the page.

```html
<!-- input "<script>alert(1)</script>" echoed into the page runs as code -->
```

* **Reflected** (in the immediate response, via a link), **Stored** (saved and
  served to every viewer — e.g. a comment), **DOM-based** (client-side JS writes
  input into the DOM).
* **Find it (lab):** inject a harmless marker like
  `"><script>alert(document.domain)</script>` and see if it executes; test each
  reflection context (HTML body, attribute, JS, URL).
* **Fix:** **context-aware output encoding**, a strict **Content-Security-Policy
  (CSP)**, and framework auto-escaping (e.g. React/Jinja). Set cookies
  `HttpOnly` so script can't read them.

### Command injection

User input reaches a shell command, so the input can add commands:

```python
os.system("ping -c 1 " + host)      # host = "8.8.8.8; cat /etc/passwd"
```

* **Find it (lab):** append `; id`, `| id`, `$(id)`, or `` `id` `` and look for
  command output or a time delay (`; sleep 5`).
* **Fix:** avoid the shell — pass arguments as an array to `execve`-style APIs,
  never interpolate into a shell string; validate against an allow-list.

### Path traversal / LFI

Input used to build a file path lets `../` escape the intended directory:

```
GET /download?file=../../../../etc/passwd
```

* **Fix:** resolve the canonical path and confirm it stays within the allowed
  base directory; never pass user input straight to file APIs.

### Server-side request forgery (SSRF)

The server fetches a URL supplied by the user, so you can make *the server*
request internal resources it can reach but you can't — cloud metadata
endpoints, internal admin panels, `localhost` services.

```
POST /fetch   url=http://169.254.169.254/latest/meta-data/   (cloud metadata)
```

* **Fix:** allow-list the destinations the server may fetch; block internal/link-
  local ranges; don't follow redirects blindly.

### Broken access control (IDOR)

The app checks *authentication* (who you are) but not *authorisation* (whether
you may access this object). Changing an identifier reaches someone else's data:

```
GET /invoice?id=1001     ->  change to 1002 and read another user's invoice
```

* **Find it (lab):** increment/swap IDs, UUIDs, or filenames; replay another
  user's request with your own session.
* **Fix:** enforce an ownership/authorization check **server-side** on every
  object access — never rely on the client not changing a value.

### Authentication & session weaknesses

Weak or missing controls on login and sessions: no rate limiting (credential
stuffing / brute force), predictable or non-rotated session IDs, secrets in the
URL, missing logout invalidation, JWTs accepted with `alg: none`.

* **Fix:** rate-limit and lock out; use long random session IDs; rotate the
  session ID on login; set cookies `Secure; HttpOnly; SameSite`; verify JWT
  signatures and pin the algorithm.

### Cross-site request forgery (CSRF)

Because the browser auto-attaches cookies, a malicious page can make a victim's
browser send an authenticated state-changing request (e.g. "change my email")
without their intent.

* **Fix:** anti-CSRF tokens on state-changing requests and `SameSite` cookies.

### Insecure file upload

If the server stores an upload in a web-reachable path and will execute it, an
attacker uploads a server-side script (e.g. a `.php` webshell) and requests it.

* **Fix:** validate type/content, store outside the web root or on object
  storage, randomise names, and never serve uploads as executable.

## A rough methodology

1. **Map** the app — crawl pages, note every parameter, cookie and endpoint
   (DevTools + Burp proxy history; `ffuf` for hidden paths).
2. **Understand** each feature: what does this input *do* on the server?
3. **Probe** inputs for the classes above (one `'`, one `<script>`, one `../`,
   one changed ID…).
4. **Confirm & assess impact**, then **report** (for CTFs: grab the flag).

## Learn by doing

Deliberately vulnerable targets built for practice:

* [PortSwigger Web Security Academy](https://portswigger.net/web-security) —
  free, best-in-class labs with explanations (start here).
* [OWASP Juice Shop](https://owasp.org/www-project-juice-shop/) — a full modern
  vulnerable app.
* [DVWA](https://github.com/digininja/DVWA) — "Damn Vulnerable Web App", classic
  for the basics with adjustable difficulty.
* [OWASP Top 10](https://owasp.org/www-project-top-ten/) — the canonical list of
  these risk categories and how they're mitigated.
