#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <string>

httplib::Client monzo_endpoint("https://api.monzo.com");

namespace monzo {
    class Client {
        public:
            Client(std::string token) {
                headers = {
                    {"Authorization", "Bearer " + token}
                };
            }

            std::string WhoAmI() {
                std::string path = "/ping/whoami";
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string GetAccounts() {
                std::string path = "/accounts";
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string GetBalance(const std::string &accountId) {
                std::string path = "/balance?account_id=" + accountId;
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string GetPots(const std::string &accountId) {
                std::string path = "/pots?current_account_id=" + accountId;
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string DepositIntoPot(const std::string &destination_pot_id, const std::string &source_account_id, const int &amount, const std::string &dedupe_id) {
                std::string path = "/pots/" + destination_pot_id + "/deposit";
                std::string body = "amount=" + std::to_string(amount) + "&source_account_id=" + source_account_id + "&dedupe_id=" + dedupe_id;
                auto response = monzo_endpoint.Put(path, headers, body, "application/x-www-form-urlencoded");
                return response->body;
            }

            std::string WithdrawFromPot(const std::string &source_pot_id, const std::string &destination_account_id, const int &amount, const std::string &dedupe_id) {
                std::string path = "/pots/" + source_pot_id + "/withdraw";
                std::string body = "amount=" + std::to_string(amount) + "&source_account_id=" + destination_account_id + "&dedupe_id=" + dedupe_id;
                auto response = monzo_endpoint.Put(path, headers, body, "application/x-www-form-urlencoded");
                return response->body;
            }

            std::string GetTransactionsInTimePeriod(const std::string &account_id, const std::string &start_time, const std::string &end_time) {
                std::string path = "/transactions?account_id=" + account_id + "&since=" + start_time + "&before=" + end_time;
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string GetAllTransactions(const std::string &account_id) {
                std::string path = "/transactions?account_id=" + account_id;
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string GetSingleTransaction(const std::string &transaction_id, const bool &expandMerchant = false) {
                std::string path = "/transactions/" + transaction_id;
                if (expandMerchant == true) {
                    path.append("/?expand[]=merchant");
                }
                auto response = monzo_endpoint.Get(path, headers);
                return response->body;
            }

            std::string AnnotateTransaction(const std::string &transaction_id, const std::string &key, const std::string &value) {
                std::string path = "/transactions/" + transaction_id;
                std::string body = "metadata[" + key + "]=" + value;
                auto response = monzo_endpoint.Patch(path, headers, body, "application/x-www-form-urlencoded");
                return response->body;
            }

            std::string CreateFeedItem(const std::string &account_id, const std::string &feed_item_title, const std::string &feed_item_image_url, const std::string &feed_item_body = "", const std::string &feed_item_interact_url = "", const std::string &feed_item_type = "basic") {
                std::string path = "/feed";
                std::string body = "account_id=" + account_id + "&type=" + feed_item_type + "&url=" + feed_item_interact_url + "&params[title]=" + feed_item_title + "&params[image_url]=" + feed_item_image_url + "&body=" + feed_item_body;
                auto response = monzo_endpoint.Post(path, headers, body, "application/x-www-form-urlencoded");
                return response->body;
            }
            
        private:
            httplib::Headers headers;
    };

    std::string ExchangeAuthorizationCode(const std::string &client_id, const std::string &client_secret, const std::string &redirect_uri, const std::string &authorization_code) {
        std::string path = "/oauth2/token";
        std::string body = "grant_type=authorization_code&client_id=" + client_id + "&client_secret=" + client_secret + "&redirect_uri=" + redirect_uri  + "&code=" + authorization_code;
        auto response = monzo_endpoint.Post(path, body, "application/x-www-form-urlencoded");
        return response->body;
    }

    std::string RefreshAccess(const std::string &client_id, const std::string &client_secret, const std::string &refresh_token) {
        std::string path = "/oauth2/token";
        std::string body = "grant_type=refresh_token&client_id=" + client_id + "&client_secret=" + client_secret + "&refresh_token=" + refresh_token;
        auto response = monzo_endpoint.Post(path, body, "application/x-www-form-urlencoded");
        return response->body;
    }

};