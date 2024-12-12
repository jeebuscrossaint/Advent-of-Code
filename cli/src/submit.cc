#include "aocli.hh"

// helper functio nto extract response msesage from HTML
std::string extractResponseMessage(const std::string& html) {
    GumboOutput* output = gumbo_parse(html.c_str());
    std::string message;

    // Function to recursively search for main article content
    std::function<void(GumboNode*)> findMainArticle;
    findMainArticle = [&message, &findMainArticle](GumboNode* node) {
        if (node->type != GUMBO_NODE_ELEMENT) {
            return;
        }

        // Look for <article> or <main> tags
        if (node->v.element.tag == GUMBO_TAG_ARTICLE ||
            node->v.element.tag == GUMBO_TAG_MAIN) {
            message = extractText(node);
            return;
        }

        // If we haven't found it yet, search children
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            findMainArticle(static_cast<GumboNode*>(children->data[i]));
            if (!message.empty()) {
                break;
            }
        }
    };

    findMainArticle(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    // If we couldn't find the message in the expected location,
    // try to at least get something useful
    if (message.empty()) {
        message = "Unable to parse server response. Please check the answer on the website.";
    }

    return message;
}

SubmitResponse parseResponse(const std::string& html) {
    std::string message = extractResponseMessage(html);
    SubmitResponse response;
    response.message = message;

    if (message.find("That's the right answer") != std::string::npos) {
        response.result = SubmitResult::CORRECT;
    }
    else if (message.find("too high") != std::string::npos) {
        response.result = SubmitResult::TOO_HIGH;
    }
    else if (message.find("too low") != std::string::npos) {
        response.result = SubmitResult::TOO_LOW;
    }
    else if (message.find("wait") != std::string::npos) {
        response.result = SubmitResult::RATE_LIMITED;
    }
    else if (message.find("not the right answer") != std::string::npos) {
        response.result = SubmitResult::INCORRECT;
    }
    else {
        response.result = SubmitResult::ERROR;
    }

    return response;
}

SubmitResponse submitAnswer(int year, int day, int part, const std::string& answer, const std::string& cookie) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize curl");
    }

    std::string url = "https://adventofcode.com/" +
                      std::to_string(year) + "/day/" +
                      std::to_string(day) + "/answer";

    // Prepare form data
    std::string postData = "level=" + std::to_string(part) + "&answer=" + answer;
    std::string response;

    try {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Set the cookie
        std::string cookieStr = "session=" + cookie;
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookieStr.c_str());

        // Follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Set user agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "github.com/your-username/aocli v1.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("Failed to submit answer: " +
                                   std::string(curl_easy_strerror(res)));
        }

        // Debug: Print raw response
       // std::cerr << "Debug - Raw response:\n" << response << std::endl;

        SubmitResponse result = parseResponse(response);

        // If message is empty, something went wrong with parsing
        if (result.message.empty()) {
            result.result = SubmitResult::ERROR;
            result.message = "Failed to parse response from server";
        }

        return result;
    }
    catch (...) {
        curl_easy_cleanup(curl);
        throw;
    }
}
