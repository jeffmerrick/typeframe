import type { ReactNode } from "react";
import Link from "@docusaurus/Link";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import Layout from "@theme/Layout";
import Heading from "@theme/Heading";
import ThemedImage from "@theme/ThemedImage";
import Image from "@theme/IdealImage";

import styles from "./index.module.css";
import useBaseUrl from "@docusaurus/useBaseUrl";

function FrameCard() {
  return (
    <>
      <div id="ps-85" className={styles.frame}>
        <div className={styles.frameImage}>
          <div className={styles.frameImage__large}>
            <Image
              img={require("./img/typeframe-ps-85-hero.png")}
              alt="Typeframe PS-85"
            />
          </div>
          <div className={styles.frameImage__simple}>
            <Image
              img={require("./img/typeframe-ps-85-hero-simple.png")}
              alt="Typeframe PS-85"
            />
          </div>
        </div>

        <p className={styles.frameOverline}>
          The Typeframe PS-85 Portable MU/TH/UR&nbsp;Terminal
        </p>
        <Heading as="h1" className={styles.frameTitle}>
          Mother knows best.
        </Heading>
        <div className="row">
          <div className="col col--4 col--offset-2">
            <p>
              <strong>Connect to MU/TH/UR without the umbilical cord.</strong>{" "}
              <br />
              When Weyland-Yutani approached us to design the Typeframe PS-85,
              we said, “How can we refuse?” They replied, “You can't. You've
              been acquired in a hostile takeover.” So we got to work on a
              portable, rugged terminal built for long-haul ship captains and
              officers who can't afford to stay tethered to the bridge.
            </p>

            <p>
              The PS-85 is a dedicated MU/TH/UR 6000 terminal with secure,
              shipwide wireless connectivity. Review priority directives, access
              system diagnostics, or consult MU/TH/UR from anywhere on the
              vessel - no fixed console, no hardline dependency. Advanced
              Weyland-Yutani-approved telemetry keeps you connected decks away
              from the core.
            </p>
            <p>
              Like all Typeframe systems, the PS-85 is built to adapt. A
              flexible expansion bay supports MU/TH/UR wireless modules and
              future Weyland-Yutani peripherals not yet cleared for
              documentation.
            </p>
          </div>
          <div className="col col--4">
            <p>
              Housed in a rugged enclosure with an integrated carrying handle,
              the PS-85 is designed to travel wherever duty takes you: the
              bridge, engineering, or inside a maintenance vent checking
              life-support systems. A semiotic standard keyboard provides
              precise, unambiguous input in low-light, high-stress environments.
            </p>
            <p>
              If you've been looking for portable MU/TH/UR access where
              authority meets reliability, look to the PS-85.
            </p>
            <p>
              <strong>Typeframe. Built for better worlds.</strong>
            </p>
            <ThemedImage
              alt="Docusaurus themed image"
              sources={{
                light: useBaseUrl("/img/logo-wy-light.svg"),
                dark: useBaseUrl("/img/logo-wy-dark.svg"),
              }}
              className={styles.frameLogo__small}
            />
          </div>
        </div>
        <div className={styles.frameButtons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/ps-85/"
          >
            Build Your PS-85
          </Link>
          <Link
            className="button button--outline button--secondary button--lg"
            to="/docs/ps-85/gallery"
          >
            View Gallery
          </Link>
        </div>
      </div>
      <div id="px-88" className={styles.frame}>
        <div className={styles.frameImage}>
          <div className={styles.frameImage__large}>
            <Image
              img={require("./img/typeframe-px-88-hero.png")}
              alt="Typeframe PX-88"
            />
          </div>
          <div className={styles.frameImage__simple}>
            <Image
              img={require("./img/typeframe-px-88-hero-simple.png")}
              alt="Typeframe PX-88"
            />
          </div>
        </div>

        <p className={styles.frameOverline}>
          The Typeframe PX-88 Portable Computing&nbsp;System
        </p>
        <Heading as="h1" className={styles.frameTitle}>
          A stacked deck.
        </Heading>
        <div className="row">
          <div className="col col--4 col--offset-2">
            <p>
              <strong>It's true. The odds are finally in your favor.</strong>{" "}
              <br />
              The Typeframe PX-88 is an integrated system that has been
              perfectly arranged to guarantee a superior outcome for the
              operator. Leave it to Typeframe to integrate these critical
              elements into one commanding machine.
            </p>
            <p>
              The PX-88 delivers all the power and specialized features expected
              from a professional system - but built around a dedicated,
              uncompromising user experience. Is it a cyberdeck or a writerdeck?
              It's whatever you need it to be. The reliable Raspberry Pi 4 B
              core handles demanding web-based editors and complex tasks with
              robust performance. The compact size belies the strength within.
            </p>
          </div>
          <div className="col col--4">
            <p>
              A mechanical keyboard provides a superior, tactile input
              experience - a professional tool unmatched by common consumer
              electronics. Furthermore, the system is designed for simple
              construction with minimal required soldering, and maintenance is
              streamlined - all internal components are easily reached via
              sliding access panels.
            </p>
            <p>
              If you have been looking for a portable, professional computer
              where input quality meets core performance, look at the PX-88.
            </p>
            <p>
              <strong>
                Typeframe. Built for your best work, built by you.
              </strong>
            </p>
            <ThemedImage
              alt="Docusaurus themed image"
              sources={{
                light: useBaseUrl("/img/logo-light.svg"),
                dark: useBaseUrl("/img/logo-dark.svg"),
              }}
              className={styles.frameLogo}
            />
          </div>
        </div>
        <div className={styles.frameButtons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/px-88/"
          >
            Build Your PX-88
          </Link>
          <Link
            className="button button--outline button--secondary button--lg"
            to="/docs/px-88/gallery"
          >
            View Gallery
          </Link>
        </div>
      </div>
    </>
  );
}

export default function Home(): ReactNode {
  const { siteConfig } = useDocusaurusContext();
  return (
    <Layout
      title={siteConfig.title}
      description="A collection of open-source hardware and software for building writerdecks/cyberdecks."
    >
      <div className="container">
        <FrameCard />
      </div>
    </Layout>
  );
}
