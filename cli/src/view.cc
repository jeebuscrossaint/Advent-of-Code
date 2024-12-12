#include <gumbo.h>
#include "aocli.hh"
#include <algorithm>

// Helper function to extract text from HTML nodes
std::string extractText(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        std::string text = node->v.text.text;
        // Trim excessive whitespace but preserve paragraph structure
        bool isWhitespace = std::all_of(text.begin(), text.end(), isspace);
        if (isWhitespace) return " ";
        return text;
    }
    else if (node->type == GUMBO_NODE_ELEMENT) {
        std::string text;
        GumboVector* children = &node->v.element.children;

        for (unsigned int i = 0; i < children->length; ++i) {
            text += extractText(static_cast<GumboNode*>(children->data[i]));
        }

        // Add appropriate line breaks based on HTML elements
        switch (node->v.element.tag) {
            case GUMBO_TAG_P:
                return "\n" + text + "\n";
            case GUMBO_TAG_BR:
                return "\n";
            case GUMBO_TAG_H1:
            case GUMBO_TAG_H2:
                return "\n" + text + "\n\n";
            case GUMBO_TAG_PRE:
                return "\n" + text + "\n";
            case GUMBO_TAG_CODE:
                return text;  // Preserve code formatting
            default:
                return text;
        }
    }
    return "";
}

std::string formatText(const std::string& text, size_t width) {
    std::string result;
    std::string current_line;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        if (current_line.empty()) {
            current_line = word;
        }
        else if (current_line.length() + word.length() + 1 <= width) {
            current_line += " " + word;
        }
        else {
            result += current_line + "\n";
            current_line = word;
        }
    }

    if (!current_line.empty()) {
        result += current_line + "\n";
    }

    return result;
}

std::string get_cached_problem(const Config& config, int year, int day) {
    fs::path problem_file = config.problems_dir / (std::to_string(year) + "_" + std::to_string(day) + ".txt");
    if (fs::exists(problem_file)) {
        std::ifstream file(problem_file);
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return content;
    }
    return "";
}

void cache_problem(const Config& config, int year, int day, const std::string& problem) {
    fs::path problem_file = config.problems_dir / (std::to_string(year) + "_" + std::to_string(day) + ".txt");
    std::ofstream file(problem_file);
    file << problem;
}

// Find the main problem description
std::string findProblemDescription(GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return "";
    }

    // Look for main article tags
    if (node->v.element.tag == GUMBO_TAG_ARTICLE ||
        node->v.element.tag == GUMBO_TAG_MAIN) {
        std::string text = extractText(node);
        // Make sure we got meaningful content
        if (text.find("Day") != std::string::npos) {
            return text;
        }
    }

    // Recurse through children
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        std::string desc = findProblemDescription(static_cast<GumboNode*>(children->data[i]));
        if (!desc.empty()) {
            return desc;
        }
    }
    return "";
}

std::string viewProblem(int year, int day, const std::string& cookie) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize curl");
    }

    std::string url = "https://adventofcode.com/" + std::to_string(year) + "/day/" + std::to_string(day);
    std::string html;

    try {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

        // Set the cookie properly for authentication
        std::string cookieStr = "session=" + cookie;
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookieStr.c_str());

        // Follow redirects if any
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Optional: Set a user agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "github.com/your-username/aocli v1.0");

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("Failed to fetch problem page: " +
                                   std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);

        // Parse HTML
        GumboOutput* output = gumbo_parse(html.c_str());
        std::string problemText = findProblemDescription(output->root);
        gumbo_destroy_output(&kGumboDefaultOptions, output);

        if (problemText.empty()) {
            throw std::runtime_error("Failed to parse problem description");
        }

        return problemText;
    }
    catch (...) {
        curl_easy_cleanup(curl);
        throw;
    }
}
