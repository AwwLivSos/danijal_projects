package etf.ri.rma.newsfeedapp.data

import android.os.Build
import androidx.annotation.RequiresApi
import etf.ri.rma.newsfeedapp.model.NewsItem
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter
import java.time.format.DateTimeFormatter.ISO_DATE_TIME
import etf.ri.rma.newsfeedapp.model.Tag

object NewsData {
    private val initialNews = mutableListOf<NewsItem>()
    private var isInitialized = false

    @RequiresApi(Build.VERSION_CODES.O)
    fun getAllNews(): List<NewsItem> {
        if (!isInitialized) {
            initializeNews()
            isInitialized = true
        }
        return initialNews.toList()
    }

    fun addNews(news: NewsItem) {
        initialNews.add(news)
    }

    fun clearCategory(category: String) {
        initialNews.removeAll { it.category == category }
    }

    @RequiresApi(Build.VERSION_CODES.O)
    private fun initializeNews() {
        // Format the published_at date to our format (yyyy-MM-dd)
        val inputFormatter = ISO_DATE_TIME
        val outputFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd")

        initialNews.addAll(listOf(
            NewsItem(
                uuid = "24c9a292-2811-4503-b356-597a3df9d44e",
                title = "Biden aides were willing to do 'undemocratic things' to keep faltering president in power: 'Original Sin' author",
                snippet = "Joe Biden's aides were willing to do \"undemocratic things\" to cover up the former president's mental decline and keep him in power for another four year...",
                imageUrl = "https://nypost.com/wp-content/uploads/sites/2/2025/05/newspress-collage-2t0sx891y-1748297477051.jpg?quality=75&strip=all&1748283090&w=1024",
                category = "politics", // Mapped from "general" since it's a political story
                isFeatured = true,
                source = "nypost.com",
                publishedDate = LocalDateTime.parse("2025-05-26T22:32:50.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "politics"), Tag(value = "us-news"), Tag(value = "books"), Tag(value = "donald-trump"), Tag(value = "jake-tapper"), Tag(value = "joe-biden"), Tag(value = "white-house")
                )
            ),
            NewsItem(
                uuid = "a1aec59a-4d2b-47cc-b346-9baf11f80cf0",
                title = "Shocking video shows moment e-bike slams into girl, 3, in NYC bike lane",
                snippet = "Shocking video captured the moment an e-bike rider slammed into a 3-year-old girl as she ran toward a Brooklyn bike lane over the weekend.",
                imageUrl = "https://nypost.com/wp-content/uploads/sites/2/2025/05/105371224.jpg?quality=75&strip=all&w=1024",
                category = "general",
                isFeatured = true,
                source = "nypost.com",
                publishedDate = LocalDateTime.parse("2025-05-26T22:25:25.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "metro"), Tag(value = "us-news"), Tag(value = "bikes"), Tag(value = "brooklyn"), Tag(value = "children"), Tag(value = "crime"), Tag(value = "toddlers"), Tag(value = "williamsburg")
                )
            ),
            NewsItem(
                uuid = "d23a0edd-6b4b-4157-87ce-08498d59d6ed",
                title = "Cornell tops Maryland for first NCAA lacrosse title since 1977",
                snippet = "Cornell, the first NCAA lacrosse champion, ended a 48-year drought with a 13-10 win over Maryland on Monday for the Big Red's fourth national championship.",
                imageUrl = "https://a2.espncdn.com/combiner/i?img=%2Fphoto%2F2025%2F0526%2Fr1498742_1296x729_16%2D9.jpg",
                category = "sports",
                isFeatured = true,
                source = "espn.com",
                publishedDate = LocalDateTime.parse("2025-05-26T22:18:17.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "sports"), Tag(value = "college-sports"), Tag(value = "lacrosse"), Tag(value = "ncaa"), Tag(value = "cornell"), Tag(value = "maryland")
                )
            ),
            NewsItem(
                uuid = "7411749c-501f-40d8-93ab-5d4ec98aaa57",
                title = "Selling Sunset's Chrishell Stause Reveals Why Friendship With Emma Ended",
                snippet = "Chrishell Stause explains the fallout with Emma Hernan, reportedly triggered by comments made by Hernan's MAGA boyfriend.",
                imageUrl = "https://www.usmagazine.com/wp-content/uploads/2025/05/chrishell-stause-2211519901.jpg?crop=0px%2C351px%2C3524px%2C1850px&resize=1200%2C630&quality=86&strip=all",
                category = "entertainment",
                isFeatured = true,
                source = "usmagazine.com",
                publishedDate = LocalDateTime.parse("2025-05-26T23:05:17.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "selling-sunset"), Tag(value = "reality-tv"), Tag(value = "celebrity"), Tag(value = "chrishell-stause"), Tag(value = "emma-hernan"), Tag(value = "drama")
                )
            ),
            NewsItem(
                uuid = "f3ab95c0-7258-4066-87ec-f995a431cff4",
                title = "US Coast Guard's Fleet Week demonstration in NYC nixed due to real offshore search mission on Memorial Day",
                snippet = "Coast Guard canceled a Fleet Week rescue demo to respond to a real distress call 20 miles off the Jersey shore.",
                imageUrl = "https://nypost.com/wp-content/uploads/sites/2/2025/05/newspress-collage-xgzf9xuxa-1748294384968.jpg?quality=75&strip=all&1748279998&w=1024",
                category = "general",
                isFeatured = false,
                source = "nypost.com",
                publishedDate = LocalDateTime.parse("2025-05-26T23:03:17.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "coast-guard"), Tag(value = "fleet-week"), Tag(value = "search-and-rescue"), Tag(value = "memorial-day"), Tag(value = "nyc"), Tag(value = "news")
                )
            ),
            NewsItem(
                uuid = "305c87d5-2819-40f6-b873-084812b3a5a7",
                title = "Nassau County holds Memorial Day tributes to fallen troops with parade, moment of silence: 'Say a prayer for them'",
                snippet = "Long Island honored fallen soldiers with a parade, moment of silence, and heartfelt community events.",
                imageUrl = "https://nypost.com/wp-content/uploads/sites/2/2025/05/105373386.jpg?quality=75&strip=all&w=1024",
                category = "general",
                isFeatured = false,
                source = "nypost.com",
                publishedDate = LocalDateTime.parse("2025-05-26T22:56:00.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "nassau"), Tag(value = "memorial-day"), Tag(value = "veterans"), Tag(value = "long-island"), Tag(value = "patriotism"), Tag(value = "usa")
                )
            ),
            NewsItem(
                uuid = "16260e18-4d5c-4d0c-830f-a2d50a3b964d",
                title = "Three more escaped New Orleans inmates captured as manhunt continues for last 2 fugitives",
                snippet = "Three more fighters on the run after a group of 10 inmates escaped from a New Orleans jail have been captured, authorities said Monday.",
                imageUrl = "https://nypost.com/wp-content/uploads/sites/2/2025/05/newspress-collage-g7xlcy89e-1748331000336.jpg?quality=75&strip=all&1748316664&w=1024",
                category = "crime", // Mapped from general based on content
                isFeatured = false,
                source = "nypost.com",
                publishedDate = LocalDateTime.parse("2025-05-27T07:43:14.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "us-news"), Tag(value = "crime"), Tag(value = "fugitives"), Tag(value = "manhunt"), Tag(value = "new-orleans")
                )
            ),
            NewsItem(
                uuid = "9f3ea814-6b83-4f7e-9132-5497038e96ec",
                title = "Trump Media Group Reportedly Plans To Raise $3 Billion To Buy Cryptocurrencies",
                snippet = "Trump Media is reportedly strategizing to raise a $3 billion investment in cryptocurrencies, echoing MicroStrategy's playbook.",
                imageUrl = "https://cdn.benzinga.com/files/images/story/2025/05/27/Konskie--Poland---February-27--2025-Trum.jpeg?width=1200&height=800&fit=crop",
                category = "business",
                isFeatured = true,
                source = "benzinga.com",
                publishedDate = LocalDateTime.parse("2025-05-27T07:34:20.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "trump"), Tag(value = "crypto"), Tag(value = "bitcoin"), Tag(value = "media"), Tag(value = "investment"), Tag(value = "mstr"), Tag(value = "business")
                )
            ),
            NewsItem(
                uuid = "48b0ab58-8de0-4ee1-9458-08904efedec1",
                title = "Diddy Seeks to Remove Kid Cudi's Car Explosion Trial Testimony",
                snippet = "Diddy's legal team argues that Kid Cudi's claims about a 2012 car explosion are speculative and prejudicial.",
                imageUrl = "https://www.usmagazine.com/wp-content/uploads/2025/05/Diddy-Asks-Judge-to-Strike-Kid-Cudis-Car-Explosion-Claims-From-the-Record.jpg?crop=0px%2C0px%2C2000px%2C1051px&resize=1200%2C630&quality=86&strip=all",
                category = "entertainment",
                isFeatured = false,
                source = "usmagazine.com",
                publishedDate = LocalDateTime.parse("2025-05-27T07:23:34.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "diddy"), Tag(value = "kid-cudi"), Tag(value = "trial"), Tag(value = "celebrity"), Tag(value = "courtroom"), Tag(value = "explosion")
                )
            ),
            NewsItem(
                uuid = "aa8490f6-c2c3-46c8-b1de-2a16331a076f",
                title = "Okta shares plunge 10% as company maintains guidance, citing macroeconomic uncertainties",
                snippet = "Todd McKinnon, CEO and co-founder of Okta, speaks during the BoxWorks 2019 Conference in San Francisco, California, on Oct. 3, 2019. Okta reported better-than-expected earnings and revenue in its quarterly report on Tuesday but kept its guidance.",
                imageUrl = "https://image.cnbcfm.com/api/v1/image/106977112-1637193181681-gettyimages-1173631578-BOXWORKS_2019.jpeg?v=1748375497&w=1920&h=1080",
                category = "business",
                isFeatured = false,
                source = "cnbc.com",
                publishedDate = LocalDateTime.parse("2025-05-27T20:06:27.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "enterprise"), Tag(value = "software"), Tag(value = "okta"), Tag(value = "cybersecurity"), Tag(value = "technology"), Tag(value = "stocks"), Tag(value = "macro")
                )
            ),
            NewsItem(
                uuid = "07c470b6-5306-4de6-a4f0-98b2c433bfac",
                title = "Nvidia's feverish recovery rally faces a reality check. What investors should know",
                snippet = "Two years ago this month, Nvidia shocked investors with sales guidance beyond anybody's wildest dreams, accelerating the stock's epic run. The seemingly unstoppable AI darling now faces a reality check.",
                imageUrl = "https://image.cnbcfm.com/api/v1/image/108148695-1747838034415-gettyimages-2215609382-AFP_47HG7CF.jpeg?v=1747838130&w=1920&h=1080",
                category = "business",
                isFeatured = true,
                source = "cnbc.com",
                publishedDate = LocalDateTime.parse("2025-05-27T20:02:22.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "nvidia"), Tag(value = "ai"), Tag(value = "stocks"), Tag(value = "markets"), Tag(value = "investment"), Tag(value = "tech"), Tag(value = "trump"), Tag(value = "microsoft"), Tag(value = "meta"), Tag(value = "amazon")
                )
            ),
            NewsItem(
                uuid = "7e413eab-fe7e-4ee0-88e6-5af656954c7a",
                title = "Hoda Kotb Not Replacing Kelly Clarkson on Talk Show: Source",
                snippet = "A source exclusively tells Us Weekly that Hoda Kotb will not be replacing Kelly Clarkson on her daytime talk show, despite rumors.",
                imageUrl = "https://www.usmagazine.com/wp-content/uploads/2025/05/Is-Hoda-Really-Taking-Over-for-Kelly-Clarkson.jpg?crop=0px%2C55px%2C1998px%2C1050px&resize=1200%2C630&quality=62&strip=all",
                category = "entertainment",
                isFeatured = false,
                source = "usmagazine.com",
                publishedDate = LocalDateTime.parse("2025-05-27T20:01:43.000000Z", inputFormatter).format(outputFormatter),
                imageTags = arrayListOf(
                    Tag(value = "hoda-kotb"), Tag(value = "kelly-clarkson"), Tag(value = "talk-show"), Tag(value = "tv"), Tag(value = "rumors"), Tag(value = "celebrity"), Tag(value = "media")
                )
            )
        ))
    }
} 