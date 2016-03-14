/*
 *  ofxInstagram
 *
 *  David Haylock 2015
 */

#include "ofVec2f.h"
#include "ofxJSON.h"

class ofxInstagram
{
public:
    struct UserInfo {
        std::string bio = "",
                    fullName = "",
                    id = "",
                    profilePicture = "",
                    username = "",
                    website = "";

        unsigned int followerCount = 0,
                     followingCount = 0,
                     mediaCount = 0;
    };

    struct Caption {
        std::string createdTime = "", id = "", text = "";
        UserInfo from;
    };

    struct Comment {
        std::string createdTime = "",
                    id = "",
                    text = "";
        UserInfo from;
    };

    struct PostMedia {
        std::string url = "";
        unsigned int width = 0, height = 0;
    };

    struct Location {
        std::string id = "", name = "";
        float latitude = 0.f, longitude = 0.f;
    };

    struct PostData {
        Caption caption;
        std::string attribution = "",
                    createdTime = "",
                    filter = "",
                    link = "",
                    type = "",
                    id = "";

        Location location;

        PostMedia imageLowResolution,
                  imageStandarResolution,
                  imageThumbnail;

        PostMedia videoLowBandwidth,
                  videoLowResolution,
                  videoStandartResolution;

        UserInfo user;
        bool userHasLiked = false;
        unsigned int likeCount = 0, commentCount = 0;

        std::vector<Comment> comments;
        std::vector<std::string> tags;
        std::vector<std::pair<ofVec2f, UserInfo>> usersInPhoto;
        std::vector<UserInfo> likes;
    };

    struct Relationship {
        std::string outgoingStatus = "", incomngStatus = "";
    };

    struct TagInfo {
        unsigned int mediaCount = 0;
        std::string name = "";
    };

    struct Pagination {
        std::string minTagID = "",
                    nextMaxID = "",
                    nextMaxTagID = "",
                    nextMinID = "",
                    nextURL = "";
    };

    struct Meta {
        std::string errorType = "",
                    code = "",
                    errorMessage = "";

        friend ostream &operator<<(ostream &output, const Meta &m)
        {
            output << "Meta: {\n";
            output << "    errorType: " << m.errorType << "\n    code:" << m.code << "\n    errorMessage: " << m.errorMessage << "\n}";
            return output;
        }
    };

    using Posts = std::pair<std::vector<PostData>, Pagination>;

public:
    ofxInstagram();

    // Setup Tokens Etc...
    void setup(std::string auth_token, std::string clientID);
    void setCertFileLocation(std::string path);

    void draw();
    void drawJSON(int x);

    void resetScroll();
    void mouseScroll(int scrollY);

    std::string getParsedJSONString() const;

    //------------- USER ENDPOINTS -------------

    // GET User Info
    UserInfo getUserInformation(std::string who = "self");

    // GET User Feed use count to limit number of returns
    Posts getUserFeed(int count = 20, std::string username = "self", std::string minID = "", std::string maxID = "");

    // GET User recent images from user pass the who as the user ID number
    Posts getUserRecentMedia(std::string who = "self", int count = 20, std::string maxTimestamp = "", std::string minTimestamp = "", std::string minID = "",
                             std::string maxID = "");

    // GET User Liked Media
    Posts getUserLikedMedia(int count = 20, string username = "self", std::string maxLikeID = "");

    // GET User Search for users
    std::vector<UserInfo> getSearchUsers(std::string query = "", int count = 20);

    //------------- RELATIONSHIP ENDPOINTS -------------

    // GET User Follows
    std::vector<UserInfo> getWhoUserFollows(std::string who = "self");

    // GET User Followed By
    std::vector<UserInfo> getWhoUserIsFollowedBy(std::string who = "self");

    // GET User Requested-by
    std::vector<UserInfo> getWhoHasRequestedToFollow(std::string who = "self");

    // GET User Relationship
    Relationship getRelationshipToUser(std::string who = "self");

    // POST User Modify Relationship
    void changeRelationshipToUser(std::string who = "self", std::string action = "");

    //------------- MEDIA ENDPOINTS -------------

    // GET Info about Media Object
    PostData getMediaInformation(std::string mediaID);

    // GET Info about Media using Shortcode
    PostData getMediaInfoUsingShortcode(std::string shortcode = "");

    // GET Media Search
    Posts searchMedia(std::string lat = "", std::string lng = "", std::string min_timestamp = "", std::string max_timestamp = "", int distance = 1000);

    // GET Popular Media
    Posts getPopularMedia();

    //------------- COMMENTS ENDPOINTS -------------

    // GET Comments on Media Object
    std::vector<Comment> getCommentsForMedia(std::string mediaID);

    //------------- LIKE ENDPOINTS -------------

    // GET List of Users who have Liked a Media Object
    std::vector<UserInfo> getListOfUsersWhoLikedMedia(std::string mediaID);

    //------------- TAG ENDPOINTS -------------

    // GET Info about tagged object
    TagInfo getInfoForTags(std::string tagname);

    // GET List of recently tagged objects
    Posts getListOfTaggedObjectsNormal(std::string tagname, int count = 20, std::string min_tagID = "", std::string max_tagID = "");
    // GET List of recently tagged objects
    Posts getListOfTaggedObjectsPagination(std::string tagname, int count = 20, std::string max_tagID = "");
    // GET Search Tags
    std::vector<TagInfo> searchForTags(std::string query);

    //------------- LOCATIONS ENDPOINTS -------------

    // GET Info about a Location
    Location getInfoAboutLocation(std::string locationID);

    // GET Recent Media from location
    Posts getRecentMediaFromLocation(std::string locationID, std::string minTimestamp = "", std::string maxTimestamp = "", std::string minID = "",
                                     std::string maxID = "");

    // GET Find Location ID
    std::vector<Location> searchForLocations(std::string distance, std::string lat, std::string lng, std::string facebook_PlacesID = "",
            std::string foursquareID = "");

    Meta getLastError() const;

private:
    const std::string m_UsersURL,
          m_MediaURL,
          m_TagsURL,
          m_LocationsURL;

    //Holds the response data for the latest request
    ofHttpResponse m_Response;

    std::string m_AuthToken;
    std::string m_ClientID;
    std::string m_ResponseData;
    std::string m_CertPath;

    int m_ScrollValue;
    ofVec2f m_ScrollAmount;
    ofVec2f m_ClickOrigin;
    ofVec2f m_ReleasePos;

private:
    std::vector<PostData> constructPostsData(const ofxJSONElement &json) const;
    PostData constructPostData(const ofxJSONElement &postJson) const;

    UserInfo constructUserInfo(const ofxJSONElement &userJson) const;
    std::vector<Comment> constructComments(const ofxJSONElement &commentsJson) const;
    Pagination constructPagination(const ofxJSONElement &paginationJson) const;

    Location constructLocation(const ofxJSONElement &locationJson) const;

    Meta constructMeta(const ofxJSONElement &metaJson) const;
};
